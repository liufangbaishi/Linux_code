#include "epoll.h"

void EpollServer::Start()
{
    _listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(_listenfd == -1) {
        ErrorLog("cretae socket");
        return;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(_listenfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        ErrorLog("bind socket");
        return;
    }
    if(listen(_listenfd, 100000) < 0) {
        ErrorLog("listen");
        return;
    }
    TraceLog("epoll server listen on %d", _port);

    _eventfd = epoll_create(100000);
    if(_eventfd == -1) {
        ErrorLog("epoll_create");
        return;
    }

    SetNonblocking(_listenfd);
    OPEvent(_listenfd, EPOLLIN, EPOLL_CTL_ADD);

    EventLoop();
}

void EpollServer::EventLoop()
{
    struct epoll_event events[100000];
    while(1) {
        int n = epoll_wait(_eventfd, events, 100000, 0);
        for(int i=0; i<n; i++) {
            if(events[i].data.fd == _listenfd) {
                if(events[i].data.fd == _listenfd) {
                    struct sockaddr clientaddr;
                    socklen_t len;
                    int connectfd = accept(_listenfd, &clientaddr, &len);
                    if(connectfd < 0) {
                        ErrorLog("accept");
                    }
                    ConnectEventHandle(connectfd);
                }
            }
            else if(events[i].events & EPOLLIN) {
                ReadEventHandle(events[i].data.fd);
            }
            else if(events[i].events & EPOLLOUT) {
                WriteEventHandle(events[i].data.fd);
            }
            else{
                ErrorLog("event:%d", events[i].data.fd);
            }
        }
    }
}
