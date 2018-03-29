/*************************************************************************
	> File Name: sem_test.c
	> Author: weicheng
	> Created Time: Sun 25 Mar 2018 08:32:04 AM PDT
 ************************************************************************/

#include "comm.h"
int main()
{
    int semid = createSemSet(1);
    initSemSet(semid, 0, 1);
    pid_t pid = fork();
    if(pid == 0)
    {
        //child
        int _semid = getSemSet(0);
        while(1)
        {
            P(_semid, 0);
            printf("A");
            fflush(stdout);
            usleep(123456);
            printf("A ");
            fflush(stdout);
            usleep(123456);
            V(_semid, 0);
        }
    }
    else
    {
        //parent
        while(1)
        {
            P(semid, 0);
            printf("B");
            fflush(stdout);
            usleep(123456);
            printf("B ");
            fflush(stdout);
            usleep(123456);
            V(semid, 0);
        }
        wait(NULL);
    }
    destorySemSet(semid);
    return 0;
}
