/*************************************************************************
	> File Name: udp_server.c
	> Author: weicheng
	> Created Time: Tue 24 Apr 2018 01:01:54 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>

int createsock(const char* ip, int port)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return 2;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if(bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("bind");
        return 3;
    }
    return sock;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s [ip] [port] \n", argv[0]);
        return 1;
    }
    int sock = createsock(argv[1], atoi(argv[2]));
    char buf[1024];
    struct sockaddr_in local;
    while(1)
    {
        socklen_t len = sizeof(local);
        ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&local, &len);
        if(s > 0)
        {
            buf[s] = 0;
            printf("[%s:%d]: %s\n", inet_ntoa(local.sin_addr), ntohs(local.sin_port), buf);
            sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&local, len);
        }
    }
    return 0;
}
