/*************************************************************************
	> File Name: rw.c
	> Author: weicheng
	> Created Time: Fri 13 Apr 2018 11:27:28 PM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
pthread_rwlock_t rwlock;
int count = 0;

void* route_write(void* arg)
{
    int i = (int)arg;
    int t = 0;
    while(1)
    {
        t = count;
        usleep(10000);
        pthread_rwlock_wrlock(&rwlock);
        printf("(write)no:(%d), id=%x, t=%d, count=%d\n", i, pthread_self(), t, ++count);
        pthread_rwlock_unlock(&rwlock);
        usleep(90000);
    }
}

void* route_read(void* arg)
{
    int i = (int)arg;
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("(read)no:(%d), id=%x, count=%d\n", i, pthread_self(), count);
        pthread_rwlock_unlock(&rwlock);
        usleep(10000);
    }
}
int main()
{
    pthread_t tid[8];
    pthread_rwlock_init(&rwlock, NULL);
    int i=0;
    for(; i<3; i++)
    {
        pthread_create(&tid[i], NULL, route_write, (void*)i);
    }

    for(i=3; i<8; i++)
    {
        pthread_create(tid+i, NULL, route_read, (void*)i);
    }

    for(i=0; i<8; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
