#ifndef _COMM_H_
#define _COMM_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
union semun 
{
    int   val;    /* value for setval */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO*/
};


int createSemSet(int nums);
int initSem(int semid, int nums, int initval);
int getSemSet(int nums);
int P(int semid, int who);
int V(int semid, int who);
int destorySemSet(int semid);

#endif
