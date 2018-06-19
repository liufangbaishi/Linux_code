/*************************************************************************
	> File Name: http.c
	> Author: weicheng
	> Created Time: Sat 16 Jun 2018 07:49:11 PM CST
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <pthread.h>
#define MAX 1024

void clear_head(int sock)
{
    char line[MAX];
    do
    {
        get_line(sock, line, MAX);
    }while(strcmp(line, "\n"));
}

int echo_www(int sock, const char* path, ssize_t size)
{
    clear_head(sock);

    int fd = open(path, O_RDONLY);
    if(fd < 0)
    {
        return 404;
    }

    char line[MAX];
    sprintf(line, "HTTP/1.0 200 OK\r\n");
    send(sock, line, strlen(line), 0);
    sprintf(line, "Content-Type:text/html;charset=UTF-8\r\n");
    send(sock, line, strlen(line), 0);
    sprintf(line, "\r\n");
    send(sock, line, strlen(line), 0);

    sendfile(sock, fd, NULL, size);
    close(fd);
    return 200;
}

int startup(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(2);
    }
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);
    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) <0)
    {
        perror("bind");
        exit(3);
    }
    if(listen(sock, 5) < 0)
    {
        perror("listen");
        exit(4);
    }
    return sock;
}

// 按行读取内容
int get_line(int sock, char line[], int num)//num是希望读取的字符个数
{
    char c = 0;
    int i = 0;
    while(i < num-1 && c != '\n') // 小于num-1，是因为还要存放\0这个字符
    {
        if(recv(sock, &c, 1, 0) > 0)
        {
            if(c == '\r')
            {
                // 读取到\r，在看下一个字符是不是\n,如果是\n，直接读取，如果不是，将\r变为\n
                if(recv(sock, &c, 1, MSG_PEEK) > 0 && c == '\n')
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            line[i++] = c;
        }
    }
    line[i] = '\0';
    return i;// 返回实际读取到的字符个数
}

void echo_error(int errcode)
{
    switch(errcode)
    {
        case 404:
            break;
        case 503:
            break;
        case 504:
            break;
        default:
            break;
    }
}

void* handler_request(void* arg)
{
    int sock = (int)arg;
    char line[MAX];
    int cgi = 0;
    int errcode = 200;
    char method[MAX/8]; //方法
    char url[MAX]; //url
    char path[MAX];
    char* query_string;// GET方法的参数

    // 提取请求的第一行
    get_line(sock, line, sizeof(line));

    int i = 0;
    int j = 0;
    while(i < sizeof(line)-1 && j < sizeof(method) && !isspace(line[i])) //得到方法
    {
        method[j++] = line[i++];
    }
    method[j] = '\0';
    if(strcasecmp(method, "POST")!=0 && strcasecmp(method, "GET")!=0) //只处理get和post方法
    {
        errcode = 404;
        goto end;
    }
    if(strcasecmp(method, "POST") == 0) //post方法
        cgi = 1;
    
    while(i < sizeof(line)-1 && isspace(line[i])) // 过滤掉空格
        i++;
    j = 0;
    while(i < sizeof(line)-1 && j < sizeof(url) && !isspace(line[i])) //得到url
    {
        url[j++] = line[i++];
    }
    url[j] = '\0';
    
    if(strcasecmp(method, "GET") == 0) // 如果是get方法，分离query_string
    {
        query_string = url;
        while(*query_string)
        {
            if(*query_string == '?')
            {
                *query_string = '\0';
                query_string++;
                cgi = 1;
                break;
            }
            query_string++;
        }
    }
  
    sprintf(path, "wwwroot%s", url);// 如果url是目录，拼接上默认的index.html
    if(path[strlen(path)-1] == '/')
        strcat(path, "index.html");

    //检测资源是否有效，以及是否有可执行权限
    struct stat st;
    if(stat(path, &st) < 0)
    {
        errcode = 404;
        goto end;
    }
    else
    {
        if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) \
                    || (st.st_mode & S_IXOTH))
            cgi = 1;
        else if(S_ISDIR(st.st_mode))
            strcat(path, "index.html");//目录文件
        else
        {}
    }

    printf("method:%s path:%s\n", method, path);
    // cgi和非cgi
    if(cgi)
    {
        //excu_cgi
    }
    else
        errcode = echo_www(sock, path, st.st_size);

end:
    if(errcode != 200)
    {
        echo_error(errcode);
    }
    close(sock);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }

    int listen_sock = startup(atoi(argv[1]));
    for(;;)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_sock = accept(listen_sock, (struct sockaddr*)&client, &len);
        if(new_sock < 0)
        {
            perror("accept");
            continue;
        }
        pthread_t tid = 0;
        pthread_create(&tid, NULL, handler_request, (void*)new_sock);
        pthread_detach(tid);
    }
    close(listen_sock);
    return 0;
}


