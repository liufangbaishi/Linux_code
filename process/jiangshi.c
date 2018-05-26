/*************************************************************************
	> File Name: proc.c
	> Author: weicheng
	> Created Time: Wed 23 May 2018 04:08:22 PM PDT
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
    {  // child
        printf("child[%d] is begin Z...\n", getpid());
        sleep(5);
        exit(0);
    }
    else
    {
        // parent
        printf("parent[%d] is sleeping ...\n", getpid());
        sleep(30);
    }
    return 0;
}
