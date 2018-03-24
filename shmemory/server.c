/*************************************************************************
	> File Name: server.c
	> Author: weicheng
	> Created Time: Fri 23 Mar 2018 11:02:28 PM PDT
 ************************************************************************/

#include "comm.h"
int main()
{
    int shmid = createShm(4096);

    char* addr = shmat(shmid, NULL, 0);
    sleep(3);

    while(1)
    {
        printf("client# %s\n", addr);
        sleep(1);
    }

    shmdt(addr);
    sleep(3);
    destoryShm(shmid);
    return 0;
}
