#include "cserver.h"
#include "clog.h"
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
        _log->log_exit(2, "socket error: %s", strerror(errno));
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVPORT);
    if (bind(_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        _log->log_exit(2, "bind error: %s", strerror(errno));
    if (listen(_listenfd, 5) < 0)
        _log->log_exit(2, "listen error: %s", strerror(errno));

    if ((_epollfd = epoll_create(EPOLLSIZE)) < 0)
        _log->log_exit(2, "epoll create error: %s", strerror(errno));
    add_event(_listenfd);
    _log->log_msg(0, "start listening");
}

void CServer::do_epoll()
{
    struct sockaddr_in cliaddr;
    socklen_t clilen;
    struct epoll_event events[EPOLLSIZE];
    int fd, ret, nfds = 0, newfd = 0;
    char buf[MAXLINE];
    std::list<int>::const_iterator pos;
    while (true) {
        if ((nfds = epoll_wait(_epollfd, events, EPOLLSIZE, -1)) < 0)
            _log->log_exit(2, "epoll_wait error: %s", strerror(errno));
        for (int i = 0; i < nfds; ++i) {
            fd = events[i].data.fd;
            if (fd == _listenfd) {
                clilen = sizeof(cliaddr);
                if ((newfd = accept(_listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
                    if (errno == EINTR)
                        continue;
                    else
                        _log->log_exit(2, "accept error: %s", strerror(errno));
                }
                add_event(newfd);
                _b_connmap[newfd] = false;
                printf("%d connected\n", newfd);
            } else {
                ret = _request.process(fd, buf, MAXLINE, _b_connmap[fd]);
                if (ret < 0) {
                    delete_event(fd);
                    _log->log_msg(1, "read error on %d: %s", fd, strerror(errno));
                } else if (ret == 0) {
                    delete_event(fd);
                    _log->log_msg(0, "%d closed", fd);
                } else {
                    if (_b_connmap[fd]) {

                    } else {

                        _b_connmap[newfd] = true;
                    }
                    write(STDOUT_FILENO, buf, ret);
                    _log->log_msg(0, buf);
//                    for (pos = _fdlist.begin(); pos != _fdlist.end(); ++i) {
//                        if (*pos != fd) {
//                            ret = _response.process(*pos, buf, ret);
//                        }
//                    }
                }
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
    close(fd);
}

