/*************************************************************************
	> File Name: pthread_mutex.c
	> Author: weicheng
	> Created Time: Mon 09 Apr 2018 05:04:39 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

int ticket = 100;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* route(void* arg)
{
    char* id = (char*)arg;
    while(1)
    {
        //pthread_mutex_lock(&lock);
        if(ticket>0)
        {
            usleep(1000);
            printf("%s sells ticket: %d\n", arg, ticket);
            ticket--;
            //pthread_mutex_unlock(&lock);
        }
        else
        {
            //pthread_mutex_unlock(&lock);
            break;
        }
    }
}

int main()
{
    pthread_t t1, t2, t3, t4;

    pthread_create(&t1, NULL, route, "thread1");
    pthread_create(&t2, NULL, route, "thread2");
    pthread_create(&t3, NULL, route, "thread3");
    pthread_create(&t4, NULL, route, "thread4");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}



//pthread_cond_t cond;
//pthread_mutex_t lock;
//void* r1(void* arg)
//{
//    while(1)
//    {
//        printf("hahahaha\n");
//        pthread_cond_wait(&cond, &lock);
//    }
//}
//
//void* r2(void* arg)
//{
//    while(1)
//    {
//        sleep(1);
//        pthread_cond_signal(&cond);
//    }
//}
//
//int main()
//{
//    pthread_t t1, t2;
//    pthread_mutex_init(&lock, NULL);
//    pthread_cond_init(&cond, NULL);
//
//    pthread_create(&t1, NULL, r1, NULL);
//    pthread_create(&t2, NULL, r2, NULL);
//
//    pthread_join(t1, NULL);
//    pthread_join(t2, NULL);
//
//    pthread_mutex_destroy(&lock);
//    pthread_cond_destroy(&cond);
//    return 0;
//}
