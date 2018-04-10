/*************************************************************************
	> File Name: mysleep.c
	> Author: weicheng
	> Created Time: Sat 31 Mar 2018 06:04:14 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
void sig_handler(int signo)
{
}
unsigned int mysleep(unsigned int seconds)
{
    struct sigaction new,old;
    unsigned int unslept = 0;
    new.sa_handler = sig_handler;
    sigemptyset(&new.sa_mask);
    new.sa_flags = 0;
    sigaction(SIGALRM, &new, &old);
    
    sigset_t newmask, oldmask, suspmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    alarm(seconds);

    suspmask = oldmask;
    sigdelset(&suspmask, SIGALRM);
    sigsuspend(&suspmask);

    unslept = alarm(0);
    sigaction(SIGALRM, &old, NULL);

    sigprocmask(SIG_UNBLOCK, &oldmask, NULL);
    return unslept;
}
int main()
{
    while(1)
    {
        printf("Hello, world\n");
        mysleep(1);
    }
    return 0;
}
