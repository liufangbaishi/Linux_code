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

#define SIZE 5
sem_t sem_blank;
sem_t sem_data;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

int ring[SIZE];

void* produce(void* arg)
{
    static int i = 0;
    while(1)
    {
        pthread_mutex_lock(&lock1);
        sem_wait(&sem_blank);
        int data = rand()%1234;
        ring[i] = data;
        i++;
        i %= SIZE;
        printf("producer done...data:%d\n", data);
        usleep(10000);
        sem_post(&sem_data);
        pthread_mutex_unlock(&lock1);
    }
}

void* consume(void* arg)
{
    static int i = 0;
    while(1)
    {
        pthread_mutex_lock(&lock2);
        sem_wait(&sem_data);
        int data = ring[i];
        i++;
        i %= SIZE;
        printf("consumemer done...data:%d\n", data);
        usleep(1000);
        sem_post(&sem_blank);
        pthread_mutex_unlock(&lock2);
    }
}
int main()
{
    srand((unsigned long)time(NULL));

    pthread_t con[3];
    pthread_t pro[3];
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    sem_init(&sem_blank, 0, SIZE);
    sem_init(&sem_data, 0, 0);

    int i=0;
    for(i=0; i<3; i++)
        pthread_create(&con[i], NULL, consume, NULL);
    for(i=0; i<3; i++)
        pthread_create(&pro[i], NULL, produce, NULL);

    for(i=0; i<3; i++)
        pthread_join(con[i], NULL);
    for(i=0; i<3; i++)
        pthread_join(pro[i], NULL);

    sem_destroy(&sem_blank);
    sem_destroy(&sem_data);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    return 0;
}
