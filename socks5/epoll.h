/*************************************************************************
	> File Name: epoll.h
	> Author: weicheng
	> Created Time: Thu 23 Aug 2018 11:15:35 AM CST
 ************************************************************************/

#ifndef _EPOLL_H__
#define _EPOLL_H__

#include "common.h"
class EpollServer
{
public:
    EpollServer(int port = 8888)
        : _port(port)
        , _listenfd(-1)
        , _eventfd(-1)
    {}

    ~EpollServer()
    {
        if(_listenfd == -1)
            close(_listenfd);
    }

    void SetNonblocking(int fd)
    {
        int flags, s;
        flags = fcntl(fd, F_GETFL, 0);
        if(flags == -1) {
            ErrorLog("SetNonblocking:F_GETFL");
        }
        flags |= O_NONBLOCK;
        s = fcntl(fd, F_SETFL, flags);
        if(s == -1) {
            ErrorLog("SetNonblocking:F_SETFL");
        }
    }
    void OPEvent(int fd, int events, int op)
    {
        struct epoll_event event;
        event.events = events;
        event.data.fd = fd;
        if(epoll_ctl(_eventfd, op, fd, &event) < 0) {
            ErrorLog("epoll_ctl(op:%d, fd:%d)", op, fd);
        }
    }

    void Start();
    void EventLoop();

    virtual void ConnectEventHandle(int connectfd) = 0;
    virtual void ReadEventHandle(int connectfd) = 0;
    virtual void WriteEventHandle(int connectfd) = 0;
protected:
    int _port;
    int _listenfd;
    int _eventfd;
};

#endif
