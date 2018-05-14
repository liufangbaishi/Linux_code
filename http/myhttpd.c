/*************************************************************************
	> File Name: myhttpd.c
	> Author: weicheng
	> Created Time: Sun 13 May 2018 01:47:28 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

int startup(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(2);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sock);
        exit(3);
    }
    if(listen(sock, 5) < 0)
    {
        perror("listen");
        close(sock);
        exit(4);
    }
    return sock;
}

void service(int sock)
{
    char buf[10240] = {0};
    ssize_t s = read(sock, buf, sizeof(buf));
    if(s < 0)
    {
        perror("read");
        exit(5);
    }
    else if(s > 0)
    {
        buf[s-1] = 0;
        const char* hello = "HTTP/1.0 200 OK\n\n<html><head><h1>Hello world!</h1></head></html>\n";
        write(sock, hello, strlen(hello));
    }
    close(sock);
}

int main(int argc, char*argv[])
{
    if(argc != 2)
    {
        printf("Usage:%s[port]\n", argv[0]);
        return 1;
    }
    int listen_sock = startup(atoi(argv[1]));
    daemon(0, 0);
    for(;;)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if(sock < 0)
        {
            perror("accept");
            continue;
        }
        service(sock);
    }
    return 0;
}
