/*************************************************************************
	> File Name: socks5.h
	> Author: weicheng
	> Created Time: Thu 23 Aug 2018 11:14:50 AM CST
 ************************************************************************/

#ifndef _TRANFER_H__
#define _TRANFER_H__

#include "common.h"
#include "epoll.h"
class TranferServer: public EpollServer
{
public:
    TranferServer(int selfport, const char*socks5ip, int socks5port)
        : EpollServer(selfport)
    {
        memset(&_socks5addr, 0, sizeof(struct sockaddr_in));
        _socks5addr.sin_family = AF_INET;
        _socks5addr.sin_port = htons(socks5port);
        _socks5addr.sin_addr.s_addr = inet_addr(socks5ip);
    }

    virtual void ConnectEventHandle(int connectfd);
    virtual void ReadEventHandle(int connectfd);
    //virtual void WriteEventHandle(int connectfd);  
protected:
    struct sockaddr_in _socks5addr;
};

#endif
