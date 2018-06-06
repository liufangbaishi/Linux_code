/*************************************************************************
	> File Name: tcpClient.c
	> Author: weicheng
	> Created Time: Thu 26 Apr 2018 05:46:21 PM PDT
 ************************************************************************/
#include<signal.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Usage %s[ip][port]\n", argv[0]);
        exit(1);
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &local.sin_addr);
    // local.sin_addr.s_addr = inet_addr(argv[1]);
    local.sin_port = htons(atoi(argv[2]));

    int ret = connect(sock, (struct sockaddr*)&local, sizeof(local));
    if(ret < 0)
    {
        printf("connect errno, error code is:%d, errstring is:%s\n", errno, strerror(errno));
        exit(2);
    }
    printf("connect success!   \n");
    while(1)
    {
        char buf[1024]={0};
        printf("client# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf));
        if(s > 0)
        {
            buf[s-1] = 0;
            if(strcmp(buf, "quit") == 0)
            {
                printf("client quit\n");
                break;
            }
            write(sock, buf, sizeof(buf));
            //read(sock, buf, sizeof(buf));
            //printf("server# %s\n", buf);
        }
    }
    close(sock);
    return 0;
}
