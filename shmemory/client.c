/*************************************************************************
	> File Name: client.c
	> Author: weicheng
	> Created Time: Fri 23 Mar 2018 11:09:58 PM PDT
 ************************************************************************/

#include "comm.h"
int main()
{
    int shmid = getShm(4096);
    sleep(3);
    char* addr = shmat(shmid, NULL, 0);
    sleep(3);

    int i = 0;
    while(i < 26)
    {
        addr[i] = 'A'+i;
        i++;
        addr[i] = 0;
        sleep(1);
    }
    shmdt(addr);

    return 0;
}
