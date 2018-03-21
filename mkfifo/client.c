/*************************************************************************
	> File Name: client.c
	> Author: weicheng
	> Created Time: Thu 15 Mar 2018 08:14:31 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
int main()
{
    int fd = open("mypipe", O_WRONLY);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    char buf[1024] = {0};
    while(1)
    {
        printf("Please Enter # ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf)-1);
        if(s > 0)
        {
            buf[s] = 0;
            write(fd, buf, strlen(buf));
        }
        else if(s == 0)
        {
            printf("read finish");
            return 0;
        }
        else 
        {
            perror("read");
            return 2;
        }
    }
    close(fd);
    return 0;
}
