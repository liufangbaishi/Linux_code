/*************************************************************************
	> File Name: test.c
	> Author: weicheng
	> Created Time: Sat 07 Apr 2018 06:11:59 AM PDT
 ************************************************************************/
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct node
{
    int data;
    struct node* next;
}node, *node_p, **node_pp;

node_p getnode(int data)
{
    node_p ret = malloc(sizeof(node));
    if(ret != NULL)
    {
        ret->data = data;
        ret->next = NULL;
        return ret;
    }
    return NULL;
}

void initlist(node_pp pphead)
{
    if(pphead == NULL)
        exit(1);
    *pphead = malloc(sizeof(node));
    (*pphead)->next = NULL;
}

int isEmpty(node_p phead)
{
    return (phead->next == NULL)?1:0;
}

void push(node_p phead, int data)
{
    node_p tmp = getnode(data);
    if(tmp != NULL)
    {
        tmp->next = phead->next;
        phead->next = tmp;
    }
}

void print(node_p phead)
{
    phead = phead->next;
    while(phead)
    {
        printf("%d  ", phead->data);
        phead = phead->next;
    }
}

void erase(node_p phead, int *x)
{

    if(isEmpty(phead))
        return;
    node_p cur = phead->next;
    phead->next = cur->next;
    *x = cur->data;
    free(cur);
}

void destroy(node_p phead)
{
    while(!isEmpty(phead))
    {
        int data;
        erase(phead, &data);
    }
    free(phead);
}

void *producer(void* arg)
{
    node_p phead = (node_p)arg;
    while(1)
    {
        int data = rand()%1000;
        pthread_mutex_lock(&mutex);
        push(phead, data);
        printf("producer done, %d\n", data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        sleep(1);
    }
}

void* consumer(void* arg)
{
    node_p phead = (node_p)arg;
    int data = 0;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(isEmpty(phead))
        {
            printf("no product, please wait...\n");
            pthread_cond_wait(&cond, &mutex);
        }
        erase(phead, &data);
        printf("consumer done, %d\n", data);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
}
//void test()
//{
//    node_p phead;
//    int data = 0;
//    initlist(&phead);
//    push(phead, 1);
//    push(phead, 2);
//    push(phead, 3);
//    print(phead);
//    erase(phead, &data);
//    print(phead);
//    destory(phead);
//}
int main()
{
    node_p phead;
    initlist(&phead);
    
    srand((unsigned long)time(NULL));
    pthread_t t1, t2;
    pthread_create(&t1, NULL, producer, (void*)phead);
    pthread_create(&t2, NULL, consumer, (void*)phead);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    destroy(phead);
    //test();
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
