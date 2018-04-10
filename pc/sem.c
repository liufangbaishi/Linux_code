/*************************************************************************
	> File Name: posix_sig.c
	> Author: weicheng
	> Created Time: Mon 09 Apr 2018 06:12:46 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<time.h>

#define SIZE 10
sem_t sem_blank;
sem_t sem_data;

int ring[SIZE];

void* produce(void* arg)
{
    int i = 0;
    while(1)
    {
        sem_wait(&sem_blank);
        int data = rand()%1234;
        ring[i] = data;
        i++;
        i %= SIZE;
        printf("producer done...data:%d\n", data);
        sleep(1);
        sem_post(&sem_data);
    }
}

void* consume(void* arg)
{
    int i = 0;
    while(1)
    {
        sem_wait(&sem_data);
        int data = ring[i];
        i++;
        i %= SIZE;
        printf("consumemer done...data:%d\n", data);
        usleep(1000);
        sem_post(&sem_blank);
    }
}
int main()
{
    srand((unsigned long)time(NULL));

    pthread_t t1, t2;
    sem_init(&sem_blank, 0, SIZE);
    sem_init(&sem_data, 0, 0);

    pthread_create(&t1, NULL, produce, NULL);
    pthread_create(&t2, NULL, consume, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem_blank);
    sem_destroy(&sem_data);

    return 0;
}
