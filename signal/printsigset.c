/*************************************************************************
	> File Name: printsigset.c
	> Author: weicheng
	> Created Time: Sat 31 Mar 2018 05:18:32 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void printsigset(sigset_t *set)
{
    int i=0;
    for(; i<32; i++)
    {
        if(sigismember(set, i))
            putchar('1');
        else
            putchar('0');
    }
    puts("");
}

int main()
{
    sigset_t s,p;
    sigemptyset(&s);
    sigaddset(&s, SIGINT);//Ctrl+C
    sigprocmask(SIG_BLOCK, &s, NULL);
    while(1)
    {
        sigpending(&p);
        printsigset(&p);
        sleep(1);
    }
    return 0;
}
