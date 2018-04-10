/*************************************************************************
	> File Name: testvolatile.c
	> Author: weicheng
	> Created Time: Sat 31 Mar 2018 09:05:09 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>

volatile int a=0;
void handler()
{
    a=1;
}

int main()
{
    signal(SIGINT, handler);
    while(!a);
    return 0;
}
