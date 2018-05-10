/*************************************************************************
	> File Name: tcpServer.c
	> Author: weicheng
	> Created Time: Thu 26 Apr 2018 05:46:02 PM PDT
 ************************************************************************/
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>

int startup(char* ip, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {  
        printf("create socket error, errno is:%d, errstring is:%s\n", errno, strerror(errno));
        exit(1);
    }

    struct sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(port);
    server_socket.sin_addr.s_addr = inet_addr(ip);

    if(bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in)) < 0)
    {
        printf("bind error, errno is:%d, errstring is:%s\n", errno, strerror(errno));
        close(sock);
        exit(2);
    }

    if(listen(sock, 5) < 0)
    {
        printf("listen error, error code is:%d, errstring is:%s\n", errno, strerror(errno));
        close(sock);
        exit(3);
    }
    printf("bind and listen success\n");
    return sock;
}

void service(int sock, char* ip, int port)
{
    while(1)
    {
        char buf[1024]={0};
        ssize_t s = read(sock, buf, sizeof(buf));
        if(s < 0)
        {
            perror("read");
            exit(5);
        }
        else if(s > 0)
        {
            buf[s-1] = 0;
            if(strcmp(buf,"quit")==0)
            {
                printf("client quit ....\n");
                break;
            }
            printf("client[%s][%d]:%s\n", ip, port, buf);
            write(sock, buf, sizeof(buf));
        }
        else //read finish
        {
            printf("client quit ....\n");
            close(sock);
            break;
        }
    }
    printf("connect end.....Please wait.....\n");
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s [ip][port]\n", argv[0]);
        exit(4);
    }
    int listen_sock = startup(argv[1], atoi(argv[2]));
    struct sockaddr_in peer;
    for(;;)
    {
        socklen_t len = sizeof(peer);
        int new_sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if(new_sock < 0)
        {
            printf("accept error, error code is:%d, errstring is:%s\n", errno, strerror(errno));
            continue;
        }
        char buf[32];
        memset(buf, 0, sizeof(buf));
        inet_ntop(AF_INET, &peer.sin_addr, buf, sizeof(buf));
        printf("connect success! ip is:%s, port is:%d\n", buf, ntohs(peer.sin_port));

        service(new_sock, buf, ntohs(peer.sin_port));
        close(new_sock);
    }
    close(listen_sock);
    return 0;
}
