/*************************************************************************
	> File Name: http.c
	> Author: weicheng
	> Created Time: Sat 16 Jun 2018 07:49:11 PM CST
 ************************************************************************/

#include <signal.h>
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

int exe_cgi(int sock, char* path, char* method, char* query_string)
{
    int content_length = -1;
    char line[MAX];
    if(strcasecmp(method, "GET") == 0)
        clear_head(sock);
    else
    {
        do{
            get_line(sock, line, MAX);
            if(strncmp(line, "Content-Length: ", 16) == 0)
                content_length = atoi(line+16);
        }while(strcmp(line, "\n"));
        if(content_length == -1)
            return 404;
    }

    char method_env[MAX];
    char query_string_env[MAX];
    char content_length_env[MAX/8];
    // method, query_string,  content_length
    int input[2];
    int output[2];
    pipe(input);
    pipe(output);

    pid_t id = fork();
    if(id < 0)
    {
        return 404;
    }
    else if(id == 0)
    {
        //子进程
        //子进程通过input读数据，通过output写数据
        close(input[1]);
        close(output[0]);

        // 重定向
        dup2(input[0], 0);
        dup2(output[1], 1);

        sprintf(method_env, "METHOD=%s", method);
        putenv(method_env);
        if(strcasecmp(method, "GET") == 0)
        {
            sprintf(query_string_env, "QUERY_STRING=%s", query_string);
            putenv(query_string_env);
        }
        else
        {
            sprintf(content_length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(content_length_env);
        }

        execl(path, path, NULL);
        exit(1);
    }
    else
    {
        //父进程
        // 父进程通过input给子进程写，通过output读
        close(input[0]);
        close(output[1]);

        char c;
        int i = 0;
        if(strcasecmp(method, "POST") == 0)
        {
            for(; i<content_length; i++)
            {
                recv(sock, &c, 1, 0);
                write(input[1], &c, 1);
            }
        }

        sprintf(line, "HTTP/1.0 200 OK\r\n");
        send(sock, line, strlen(line), 0);
        sprintf(line, "Content-Type:text/html;charset=UTF-8\r\n");
        send(sock, line, strlen(line), 0);
        sprintf(line, "\r\n");
        send(sock, line, strlen(line), 0);
        while(read(output[0], &c, 1) > 0)
        {
            send(sock, &c, 1, 0);
        }

        wait(id);
        close(input[1]);
        close(output[0]);
    }
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

void echo_404(int sock)
{
    const char* path = "wwwroot/404.html";
    int fd = open(path, O_RDONLY);
    if(fd < 0)
        return ;
    char line[MAX];
    sprintf(line, "HTTP/1.0 404 NotFound\r\n");
    send(sock, line, strlen(line), 0);
    sprintf(line, "Content-Type:text/html;charset=UTF-8\r\n");
    send(sock, line, strlen(line), 0);
    sprintf(line, "\r\n");
    send(sock, line, strlen(line), 0);

    struct stat st;
    stat(path, &st);
    sendfile(sock, fd, NULL, st.st_size);
    close(fd);
}

void echo_error(int sock, int errcode)
{
    switch(errcode)
    {
        case 404:
            echo_404(sock);
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
        if(S_ISDIR(st.st_mode))// 先检测是不是目录文件，然后再看是不是可执行文件 
            strcat(path, "/index.html");
        else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
            cgi = 1;
        else
        {}
    }

    printf("method:%s path:%s\n", method, path);
    // cgi和非cgi
    if(cgi)
    {
        exe_cgi(sock, path, method, query_string);
    }
    else
        errcode = echo_www(sock, path, st.st_size);

end:
    if(errcode != 200)
    {
        echo_error(sock, errcode);
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
    //daemon(0, 0);
    //signal(SIGPIPE, SIG_IGN);
    struct sigaction act, old;
    act.sa_handler = SIG_IGN;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPIPE, &act, &old);
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
    sigaction(SIGPIPE, &old, NULL);
    return 0;
}


