/*************************************************************************
	> File Name: pthreadtest.c
	> Author: weicheng
	> Created Time: Sat 07 Apr 2018 08:31:15 AM PDT
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<sys/syscall.h>
//void* rout1(void* arg)
//{
//    int i=0;
//    for(i=0; i<10; i++)
//    {
//        pid_t tid = syscall(SYS_gettid);
//        printf("I am thread1,  id1=%d  id2=%p\n", tid, pthread_self());
//        pthread_cancel(pthread_self());
//    }
//}
//
//void* rout2(void *arg)
//{
//    int i=0;
//    for(;i<10;i++)
//    {
//        printf("I am thread2,  id1=%d  id2=%p\n", syscall(SYS_gettid), pthread_self());
//        sleep(1);
//        pthread_exit(arg);
//    }
//}
//
//int main()
//{
//    pthread_t t1, t2;
//
//    int ret1 = pthread_create(&t1, NULL, rout1, NULL);
//    int ret2 = pthread_create(&t2, NULL, rout2, NULL);
//
//    if(ret1 != 0)
//    {
//        printf("pthread_create:%s\n", strerror(ret1));
//        exit(1);
//    }
//    if(ret2 != 0)
//    {
//        printf("pthread_create:%s\n", strerror(ret2));
//        exit(2);
//    }
//
//    while(1)
//    {
//        printf("I am main thread\n");
//        sleep(3);
//    }
//    return 0;
//}
//


//void* thread1(void* arg)
//{
//    printf("thread is running\n");
//    int *p = (int*)malloc(sizeof(int));
//    *p = 1;
//    return (void*)p;
//}
//
//void* thread2(void* arg)
//{
//    printf("thread is running\n");
//    int *p = (int*)malloc(sizeof(int));
//    *p = 2;
//    pthread_exit((void*)p);
//}
//
//void* thread3(void* arg)
//{
//    while(1)
//    {
//        printf("thread3 3 3  is running\n");
//        sleep(1);
//    }
//    return NULL;
//}
//int main()
//{
//    pid_t tid;
//    void* ret;
//    pthread_create(&tid, NULL, thread1, NULL);
//    pthread_join(tid, &ret);
//    printf("thread is running, thread id:%x, return code:%d\n", tid, *(int*)ret);
//    free(ret);
//
//    pthread_create(&tid, NULL, thread2, NULL);
//    pthread_join(tid, &ret);
//    printf("thread is running, thread id:%x, return code:%d\n", tid, *(int*)ret);
//    free(ret);
//
//    pthread_create(&tid, NULL, thread3, NULL);
//    sleep(3);
//    pthread_cancel(tid);
//    pthread_join(tid, &ret);
//    printf("return code:%d\n", ret);
//   // if(ret == PTHREAD_CANCELED)
//    //
//   //     printf("thread is running, thread id:%x, return code:PTHREAD_CANCELED\n", tid);
//   // else
//   //     printf("thread is running, thread id:%x, return code:NULL\n", tid);
//    return 0;
//}
//


void* thread(void* arg)
{
    pthread_detach(pthread_self());
    printf("%s\n", (char*)arg);
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread, "thread is running...");

    int ret = 0;
    sleep(1);

    if(pthread_join(tid, NULL)==0)
    {
        printf("wait success\n");
        ret = 0;
    }
    else
    {
        printf("wait falied\n");
        ret = 1;
    }

    return ret;
}
