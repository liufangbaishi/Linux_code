/*************************************************************************
	> File Name: udp_client.c
	> Author: weicheng
	> Created Time: Tue 24 Apr 2018 01:03:52 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [ip] \n", argv[0]);
        return 1;
    }
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        return 2;
    }

    struct sockaddr_in peer;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(8080);
    peer.sin_addr.s_addr = inet_addr(argv[1]);

    char buf[1024];
    struct sockaddr_in server;
    while(1)
    {
        socklen_t len = sizeof(server);
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if(s > 0)
        {
            buf[s-1] = 0;
            sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&peer, sizeof(peer));
            ssize_t _s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&server, &len);
            if(_s > 0)
            {
                buf[_s] = 0;
                printf("server echo: %s\n", buf);
            }
        }
    }
    return 0;

}
