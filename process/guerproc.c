/*************************************************************************
	> File Name: proc.c
	> Author: weicheng
	> Created Time: Wed 23 May 2018 09:40:14 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return 1;
    }
    else if(id == 0)
    {
        printf("I am child, pid is %d\n", getpid());
        sleep(10);
    }
    else
    {// parent
        printf("I am parent, pid is %d\n", getpid());
        sleep(3);
        exit(0);
    }
    return 0;
}
