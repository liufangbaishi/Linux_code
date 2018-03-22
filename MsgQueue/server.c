/*************************************************************************
	> File Name: server.c
	> Author: weicheng
	> Created Time: Tue 20 Mar 2018 06:15:34 AM PDT
 ************************************************************************/

#include "comm.h"

int main()
{
    int msgid = createMsgQueue();

    char buf[1024] = {0};
    while(1)
    {
        recvMsgQueue(msgid, CLIENT_TYPE, buf);
        if(strcasecmp("quit", buf) == 0)
            break;
        printf("client# %s\n", buf);

        printf("Please enter# ");
        fflush(stdout);
        ssize_t s = read(0, buf, sizeof(buf));
        if(s>0)
        {
            buf[s-1]=0;
            sendMsgQueue(msgid, SERVER_TYPE, buf);
            printf("send done, wait recv...\n");
        }
    }

    destoryMsgQueue(msgid);
    return 0;
}
