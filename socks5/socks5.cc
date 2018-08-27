#include "socks5.h"

void Socks5Server::ConnectEventHandle(int connectfd)
{
    TraceLog("new connect event:%d", connectfd);

    SetNonblocking(connectfd);
    OPEvent(connectfd, EPOLLIN, EPOLL_CTL_ADD);
}

void Socks5Server::ReadEventHandle(int connectfd)
{
    TraceLog("connect event:%d", connectfd);
}

void Socks5Server::WriteEventHandle(int connectfd)
{
    TraceLog("");
}

int main()
{
    Socks5Server server;
    server.Start();
}
