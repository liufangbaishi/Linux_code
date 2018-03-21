/*************************************************************************
	> File Name: server.c
	> Author: weicheng
	> Created Time: Mon 12 Mar 2018 07:37:51 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
    umask(0);
    if(mkfifo("mypipe", 0644) < 0)
    {
        perror("mkfifo");
        return 1;
    }
    int fd = open("mypipe", O_RDONLY);
    if(fd < 0)
    {
        perror("open");
        return 2;
    }

    char buf[1024] = {0};
    while(1)
    {
        printf("Please wait...\n");
        ssize_t s = read(fd, buf, sizeof(buf)-1);
        if(s < 0)
        {
            perror("read");
            return 3;
        }
        else if(s == 0)
        {
            printf("client is quit\n");
            return 0;
        }
        else
        {
            buf[s-1] = 0;//为了去掉输入时的换行符，所以才s-1
            printf("client says # %s\n", buf);
        }
    }
    close(fd);
    return 0; 
}
