/*************************************************************************
	> File Name: epoll.h
	> Author: weicheng
	> Created Time: Thu 23 Aug 2018 11:15:35 AM CST
 ************************************************************************/

#ifndef _EPOLL_H__
#define _EPOLL_H__

#include "common.h"
#include "encry.h"

class IgnoreSigPipe
{
public:
    IgnoreSigPipe()
    {
        ::signal(SIGPIPE, SIG_IGN);
    }
};

static IgnoreSigPipe initPIPE_IGN;


class EpollServer
{
public:
    EpollServer(int port = 8000)
        : _port(port)
        , _listenfd(-1)
        , _eventfd(-1)
    {}

    virtual ~EpollServer()
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

    enum Sock5State
    { 
        AUTH, 
        ESTABLISHMENT, 
        FORWARDING
    };
    struct Channel
    {
        int _fd;
        string _buff;
        Channel()
            : _fd(-1)
        {}
    };
    struct Connect
    {
        Sock5State _state;
        Channel _clientChannel;
        Channel _serverChannel;
        int _ref;

        Connect()
            : _state(AUTH)
            , _ref(0)
        {}
    };
    void Start();
    void EventLoop();

    void SendInLoop(int fd, const char* buf, int len);
    void Forwarding(Channel* clientChannel, Channel* serverChannel, bool sendencry, bool recvdecrypt);
    void RemoveConnect(int fd);

    virtual void ConnectEventHandle(int connectfd) = 0;
    virtual void ReadEventHandle(int connectfd) = 0;
    virtual void WriteEventHandle(int connectfd);
protected:
    int _port;
    int _listenfd;
    int _eventfd;
    map<int, Connect*> _fdConnectMap;
};

#endif
