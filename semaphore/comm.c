/*************************************************************************
	> File Name: comm.c
	> Author: weicheng
	> Created Time: Sat 24 Mar 2018 07:41:10 AM PDT
 ************************************************************************/

#include "comm.h"

static int commSemSet(int nums, int flags)
{
    key_t key = ftok("/tmp", 0x6666);
    if(key < 0)
    {
        perror("ftok");
        return -1;
    }
    int semid = semget(key, nums, flags);
    if(semid < 0)
    {
        perror("semget");
        return -2;
    }
    return semid;
}
int createSemSet(int nums)
{
    return commSemSet(nums, IPC_CREAT|IPC_EXCL|0666);
}
int getSemSet(int nums)
{
    return commSemSet(nums, IPC_CREAT);
}
int initSemSet(int semid, int nums, int initval)
{
    union semun _un;
    _un.val = initval;
    if(semctl(semid, nums, SETVAL, _un) < 0)
    {
        perror("semctl");
        return -1;
    }
    return 0;
}

static int commPV(int semid, int who, int op)
{
    struct sembuf _sf;
    _sf.sem_num = who;
    _sf.sem_op = op;
    _sf.sem_flg = 0;
    if(semop(semid, &_sf, 1) < 0)
    {
        perror("semop");
        return -1;
    }
    return 0;
}
int P(int semid, int who)
{
    return commPV(semid, who, -1);
}
int V(int semid, int who)
{
    return commPV(semid, who, 1);
}
int destorySemSet(int semid)
{
    int tmp = semctl(semid, 0, IPC_RMID);
    if(tmp < 0)
    {
        perror("destory");
        return -1;
    }
    return 0;
}
