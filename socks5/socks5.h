/*************************************************************************
	> File Name: socks5.h
	> Author: weicheng
	> Created Time: Thu 23 Aug 2018 11:14:50 AM CST
 ************************************************************************/

#ifndef _SOCKS5_H__
#define _SOCKS5_H__

#include "epoll.h"
class Sock5Server: public EpollServer
{
public:
    Sock5Server(int port)
        : EpollServer(port)
    {}

    int AuthHandle(int fd);
    int EstablishmentHandle(int fd);

    virtual void ConnectEventHandle(int connectfd);
    virtual void ReadEventHandle(int connectfd);
    //virtual void WriteEventHandle(int connectfd);  
};

#endif
