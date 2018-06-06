/*************************************************************************
	> File Name: select_server.c
	> Author: weicheng
	> Created Time: Sat 02 Jun 2018 07:23:48 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<string.h>

#define MAX_FDS sizeof(fd_set)*8
#define INIT_DATA -1
static void InitArray(int arr[], int num)
{
    int i=0;
    for(; i<num; i++)
        arr[i] = INIT_DATA;
}

static int addSockToArray(int arr[], int num, int fd)
{
    int i=0;
    for(; i<num; i++)
    {
        if(arr[i] < 0)
        {   
            arr[i] = fd;
            return i;
        }
    }
    return -1;
}


int setArrayToFdSet(int arr[], int num, fd_set* rfds)
{
    int i=0;
    int max_fd = INIT_DATA;
    for(; i<num; i++)
    {
        if(arr[i] >= 0)
        {
            FD_SET(arr[i], rfds);
            if(max_fd < arr[i])
                max_fd = arr[i];
        }
    }
    return max_fd;
}

int startup(int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("sock");
        exit(2);
    }
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

static void serviceIO(int arr[], int num, fd_set* rfds)
{
    int i = 0;
    for(; i<num; i++)
    {
        if(arr[i] > INIT_DATA)
        {
            int fd = arr[i];
            if(i==0 && FD_ISSET(arr[i], rfds))//listen ready
            {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int sock = accept(fd, (struct sockaddr*)&client, &len);
                if(sock < 0)
                {
                    perror("accept");
                    continue;
                }

                printf("get a new client [%s:%d]\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                if(addSockToArray(arr, num, sock) == -1)
                {
                    close(sock);
                }
            }
            else if(i != 0 && FD_ISSET(arr[i], rfds))
            {   // normal fd ready, fd
                char buf[10240];
                ssize_t s = read(fd, buf, sizeof(buf)-1);
                if(s > 0)
                {
                    buf[s] = 0;
                    printf("client:> %s\n", buf);
                    const char* hello = "HTTP/1.0 200 OK\n\n<html><head><h1>Hello world!</h1></head></html>\n";
                    write(fd, hello, strlen(hello));
                    close(fd);
                    arr[i] = INIT_DATA;
                }
                else if(s == 0)
                {
                    close(fd);
                    arr[i] = INIT_DATA;
                    printf("client quit\n");
                }
                else
                {
                    perror("read");
                    close(fd);
                    arr[i] = INIT_DATA;
                }
            }
            else
            {
                //do nothing    not ready
            }
        }
    }
}


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }
    int listen_sock = startup(atoi(argv[1]));

    int fd_array[MAX_FDS];
    InitArray(fd_array, MAX_FDS);
    addSockToArray(fd_array, MAX_FDS, listen_sock);
    fd_set rfds;

    for(;;)
    {
        FD_ZERO(&rfds);
        int max_fd = setArrayToFdSet(fd_array, MAX_FDS, &rfds);

        struct timeval timeout = {3, 0};
        switch(select(max_fd+1, &rfds, NULL, NULL, NULL))
        {
            case -1:
                perror("select");
                break;
            case 0:
                printf("time out...\n");
                break;
            default:
                serviceIO(fd_array, MAX_FDS, &rfds);
                break;
        }
    }
    return 0;
}

