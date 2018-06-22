/*************************************************************************
	> File Name: epoll.c
	> Author: weicheng
	> Created Time: Mon 04 Jun 2018 06:27:55 PM CST
 ************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define MAX 128

int startup(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("sock");
        exit(2);
    }
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);
    if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
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

void serviceIO(int epoll_fd, int num, struct epoll_event* revs, int listen_sock)
{
    int i = 0;
    struct epoll_event ev;
    for(; i<num; i++)
    {
        int fd = revs[i].data.fd;
        if(revs[i].events & EPOLLIN) //处理listen_sock
        {
            if(fd == listen_sock)
            {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int new_sock = accept(fd, (struct sockaddr*)&client, &len);
                if(new_sock <0)
                {
                    perror("accpet");
                    continue;
                }
                printf("get client[%s:%d]\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                // 不能直接读，需要先等读事件就绪再读
                ev.events = EPOLLIN;
                ev.data.fd = new_sock;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_sock, &ev);
            }
            else//处理常规文件描述符
            {
                char buf[10240];
                ssize_t s = read(fd, buf, sizeof(buf)-1);
                if(s > 0)
                {
                    buf[s] = 0;//这里的读出来的数据不能保证读出来的是完整的，有可能有粘包问题
                    printf("client:> %s\n", buf);

                    //读完不能直接写，要保证准备就绪了
                    ev.events = EPOLLOUT;
                    ev.data.fd = fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev);
                }
                else if(s == 0)
                {
                    printf("client quit!\n");
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                }
                else
                {
                    perror("read\n");
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                }
            }
        }
        if(revs[i].events & EPOLLOUT)
        {
            const char* msg = "HTTP/1.0 200 OK\r\n\r\n<html><h1>Hello, my epoll proc</h1></html>";
            write(fd, msg, strlen(msg));
            // 1. 写完再开始关心读  2. 不在读，关闭连接
            close(fd);
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s[port]\n", argv[0]);
        return 1;
    }
    int listen_sock = startup(atoi(argv[1]));

    int epoll_fd = epoll_create(100);
    if(epoll_fd < 0)
    {
        perror("epoll_create");
        return 5;
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;//listen_sock上的读事件
    ev.data.fd = listen_sock;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev) < 0)
    {
        perror("epoll_ctl");
        return 6;
    }
    int timeout = -1;
    struct epoll_event revs[MAX];
    for(;;)
    {
        int size = epoll_wait(epoll_fd, revs, MAX, timeout);//返回值是多少事件就绪了,小于MAX
        switch(size)
        {
            case -1:
                perror("epoll_wait");
                break;
            case 0:
                printf("time out...\n");
                break;
            default:
                serviceIO(epoll_fd, size, revs, listen_sock);
                break;
        }
    }
    return 0;
}
