#include "cserver.h"
#include "clog.h"
#include "cprotocol.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <iterator>
#include <unistd.h>
#include <stdio.h>

CServer *CServer::_server = nullptr;

CServer::CServer()
{
    _log = CLog::log();
}

CServer *CServer::server()
{
    if (_server == nullptr)
        _server = new CServer;
    return _server;
}

void CServer::run()
{
    init();
    do_epoll();
}

void CServer::init()
{
    struct sockaddr_in servaddr;
    if ((_listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        _log->log_sys(2, "socket error");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);
    int on = 1;
    if((setsockopt(_listenfd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on))) <0)
        _log->log_sys(2, "setsockopt error");
    if (bind(_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        _log->log_sys(2, "bind error");
    if (listen(_listenfd, 5) < 0)
        _log->log_sys(2, "listen error");

    if ((_epollfd = epoll_create(EPOLLSIZE)) < 0)
        _log->log_sys(2, "epoll create error");
    add_event(_listenfd);
    _log->log_msg(0, "start listening");
}

void CServer::do_epoll()
{
    struct epoll_event events[EPOLLSIZE];
    int fd, nfds = 0;

    while (true) {
        if ((nfds = epoll_wait(_epollfd, events, EPOLLSIZE, -1)) < 0)
            _log->log_sys(2, "epoll_wait error");
        for (int i = 0; i < nfds; ++i) {
            fd = events[i].data.fd;
            if (fd == _listenfd)
                handle_accept();
            else
                handle_client(fd);
        }
    }
}

void CServer::handle_accept()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    int newfd;
    clilen = sizeof(cliaddr);
    if ((newfd = accept(_listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
        if (errno == EINTR)
            return;
        else
            _log->log_sys(2, "accept error");
    }
    add_event(newfd);
    _b_connmap[newfd] = false;
    printf("%d connected\n", newfd);
}

void CServer::handle_client(int fd)
{
    char buf[MAXLINE];
    std::list<int>::const_iterator pos;
    int nbytes = _request.process(fd, buf, MAXLINE, _b_connmap[fd]);
    if (nbytes < 0) {
        if (nbytes == -1) {
            _log->log_msg(1, "read error on %d: %s", fd, strerror(errno));
            delete_event(fd);
        } else {
            _log->log_msg(0, "ignore message from %d: %s", fd, buf);
            return;
        }
    } else if (nbytes == 0) {
        _log->log_msg(0, "%d closed", fd);
        delete_event(fd);
    } else {
        if (_b_connmap[fd]) {
            _log->log_msg(0, "recieved message from %d: %s", fd, buf);
            for (pos = _fdlist.begin(); pos != _fdlist.end(); ++pos) {
                if (*pos == fd || *pos == _listenfd)
                    continue;
                if (_response.process(*pos, buf, nbytes) < 0) {
                    _log->log_msg(1, "write error on %d: %s", fd, strerror(errno));
                    delete_event(fd);
                } else {
                    _log->log_msg(0, "sended message to %d: %s", *pos, buf);
                }
            }
        } else {
            _b_connmap[fd] = true;
            if (_response.handshark(fd, buf, nbytes) < 0) {
                _log->log_msg(1, "write error on %d: %s", fd, strerror(errno));
                delete_event(fd);
            }
        }
    }
}

void CServer::add_event(int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        if (fd == _listenfd)
            _log->log_exit(2, "listenfd add fail");
        else
            _log->log_msg(1, "%d add fail", fd);
    } else {
        _fdlist.push_back(fd);
        _log->log_msg(0, "%d added", fd);
    }
}

void CServer::delete_event(int fd)
{
    struct epoll_event ev;
    ev.events = 0;
    ev.data.fd = fd;
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, &ev);
    _fdlist.remove(fd);
    _b_connmap[fd] = false;
    close(fd);
}

