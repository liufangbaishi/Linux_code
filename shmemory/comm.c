/*************************************************************************
	> File Name: comm.c
	> Author: weicheng
	> Created Time: Fri 23 Mar 2018 10:53:58 PM PDT
 ************************************************************************/

#include "comm.h"

static int commShm(int size, int flags)
{
    key_t key = ftok("/tmp", 0x6666);
    if(key < 0)
    {
        perror("ftok");
        return -1;
    }
    int shmid = shmget(key, size, flags);
    if(shmid < 0)
    {
        perror("shmget");
        return -2;
    }
    return shmid;
}
int createShm(int size)
{
    return commShm(size, IPC_CREAT|IPC_EXCL|0666);
}
int getShm(int size)
{
    return commShm(size, IPC_CREAT);
}
int destoryShm(int shmid)
{
    if(shmctl(shmid, IPC_RMID, NULL) < 0)
    {
        perror("shmtcl");
        return -1;
    }
    return 0;
}
