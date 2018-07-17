#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;

typedef struct client {
    int cfd;
    char nickname[32];
}client_t;

typedef struct node {
    client_t c;
    struct node *next;
    struct node *prev;
}node_t;

node_t *head = NULL;
void init()
{
    node_t *p = malloc(sizeof(node_t));
    p->next = p;
    p->prev = p;
    head = p;
}

void insert(const client_t *pc)
{
    node_t *p = malloc(sizeof(node_t));
    memset(p, 0x00, sizeof(node_t));
    p->c = *pc;

    pthread_mutex_lock(&mutex);
    p->prev = head->prev;
    p->next = head;
    head->prev->next = p;
    head->prev = p;
    pthread_mutex_unlock(&mutex);
}

void travel(int cfd, const char* msg)
{
    node_t *p = head->next;
    while(p!=head) {
        char sendbuf[1024];

        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *pt = localtime(&(tv.tv_sec));
        char buf[27];
        strftime(buf, 27, "%F %T", pt);

        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        getpeername(cfd, (struct sockaddr*)&peer, &len);
        sprintf(sendbuf, "%s %s: %s", buf, inet_ntoa(peer.sin_addr), msg);
        write(p->c.cfd, sendbuf, strlen(sendbuf));
        // printf("name=%s\n", p->c.nickname);
        p = p->next;
    }
}

void erase(int cfd) 
{
    pthread_mutex_lock(&mutex);
    node_t *p = head->next;
    while(p!=head && p->c.cfd!=cfd) 
        p = p->next;

    p->next->prev = p->prev;
    p->prev->next = p->next;
    pthread_mutex_unlock(&mutex);
    free(p);
}

int length()
{
    int i = 0;
    node_t *p = head->next;
    while(p!=head){
        i++;
        p = p->next;
    }
    return i;
}

void *process(void* arg)
{
    int cfd = *(int*)arg;
    free(arg);

    char buff[1024];
    sprintf(buff, "welcome to chat room, %d online\n", length());
    write(cfd, buff, strlen(buff));
    sprintf(buff, "%s", "please input your nickname:");
    write(cfd, buff, strlen(buff));

    memset(buff, 0x00, sizeof(buff));
    ssize_t s = read(cfd, buff, 1024);
    buff[s-1] = 0;
    char msg[1024];
    sprintf(msg, "welcome %s online\n", buff);
    travel(cfd, msg);

    client_t client;
    client.cfd = cfd;
    strcpy(client.nickname, buff);
    insert(&client);

    while(1) {
        memset(buff, 0x00, sizeof(buff));
        int r = read(cfd, buff, 1024);
        if(r <= 0) break;
        travel(cfd, buff);
    }

    erase(cfd);
    travel(cfd, "bye bye");
}

int main()
{
    init();
    pthread_mutex_init(&mutex, NULL);
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1) perror("socket"), exit(1);

    int op = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int r = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    if(r == -1) perror("bind"), exit(2);

    r = listen(lfd, SOMAXCONN);
    if(r == -1) perror("listen"), exit(3);

    for(;;) {
        int cfd = accept(lfd, NULL, 0);
        if(cfd == -1) continue;

        pthread_t tid;
        int *p = malloc(sizeof(cfd));
        *p = cfd;
        pthread_create(&tid, NULL, process, p);
        pthread_detach(tid);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
