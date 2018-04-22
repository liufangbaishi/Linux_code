/*************************************************************************
	> File Name: proc.c
	> Author: weicheng
	> Created Time: Sat 14 Apr 2018 12:03:22 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
        while(1)
        {
            printf("child(%d)# I am running\n", getpid());
            sleep(1);
        }
    }
    else
    {
        int i = 5;
        while(i)
        {
            printf("paretn(%d)# I am going to died\n", getpid());
            sleep(1);
            i--;
        }
    }
    return 0;
}
