#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>

int main()
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(cfd < 0) perror("socket"),exit(1);

    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(8080);
    inet_aton("127.0.0.1", &add.sin_addr);
    int r = connect(cfd, (struct sockaddr*)&add, sizeof(add));
    if(r == -1) perror("connect"), exit(2);

    int kfd = fileno(stdin);
    int maxfd = kfd>cfd ? kfd : cfd;

    char buff[1024];
    fd_set set;
    while(1) {
        FD_ZERO(&set);
        FD_SET(cfd, &set);
        FD_SET(kfd, &set);
        int r = select(maxfd+1, &set, NULL, NULL, 0);
        if(r < 0) {
            perror("select");
            continue;
        }
        memset(buff, 0x00, sizeof(buff));

        if(FD_ISSET(cfd, &set)) {
            r = read(cfd, buff, 1024);
            if(r < 0) {
                close(cfd);
                break;
            }
            printf("%s", buff);
            fflush(stdout);
        }
        if(FD_ISSET(kfd, &set)){
            r = read(kfd, buff, 1024);
            if(r <= 0) {
                close(cfd);
                break;
            }
            write(cfd, buff, r);
        }
    }
    return 0;
}
