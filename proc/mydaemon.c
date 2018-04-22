/*************************************************************************
	> File Name: mydaemon.c
	> Author: weicheng
	> Created Time: Wed 18 Apr 2018 08:24:50 AM PDT
 ************************************************************************/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
void mydaemon()
{
    pid_t pid;
    int fd0;
    umask(0);
    if(pid = fork() < 0)
    {
        perror("fork");
        return;
    }
    else if(pid > 0)
        exit(0);
    setsid();
    signal(SIGCHLD, SIG_IGN);
    if(chdir("/") < 0)
    {
        perror("chdir");
        return;
    }
    close(0);
    //fd0 = open("/dev/null", O_RDWR);
    //dup2(fd0, 1);
    //dup2(fd0, 2);
}
int main()
{
    mydaemon();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
