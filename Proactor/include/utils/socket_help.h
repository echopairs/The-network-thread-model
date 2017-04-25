//
// Created by pairs on 4/23/17.
//

#ifndef PROACTOR_SOCKET_HELP_H
#define PROACTOR_SOCKET_HELP_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <proactor/proactor.h>
#include <vector>
#include <strings.h>

using namespace proactor;

namespace utils
{
    inline void setnonblocking(int sock)
    {
        int opts;
        opts = fcntl(sock, F_GETFL);

        if (opts < 0) {
            perror("fcntl(sock, GETFL)");
            exit(1);
        }

        opts = opts | O_NONBLOCK;

        if (fcntl(sock, F_SETFL, opts) < 0) {
            perror("fcntl(sock, SETFL, opts)");
            exit(1);
        }
    }

    inline int create_socket()
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("create socket failed");
            exit(1);
        }
        setnonblocking(sockfd);
        return  sockfd;
    }

    inline int bind_socket(int sockfd, const std::string & ip, unsigned int port)
    {
        struct sockaddr_in serveraddr;
        bzero(&serveraddr, sizeof(serveraddr));

        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr(ip.c_str());
        serveraddr.sin_port = htons(port);

        int b = bind(sockfd, (const sockaddr *) &serveraddr, sizeof(serveraddr));
        if (b < 0)
        {
            perror("bind socket failed");
            exit(1);
        }
    }


    inline int listen_socket(int listenfd)
    {
        listen(listenfd, 10);
    }

    inline ssize_t et_write(int sockfd, const char* buffer, size_t buflen)
    {
        size_t total = buflen;
        const char* p = buffer;
        ssize_t nwrite = 0;

        while(total)
        {
            nwrite = write(sockfd, p, total); // ET so we need write and write
            if (nwrite < 0)
            {
                //  when socket is no block that means write buffer is full
                if (errno == EAGAIN)
                {
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                    continue;
                }
                else
                {
                    std::cout << "write error" << std::endl;
                    return -1;
                }
            }
            else if (nwrite == total)
            {
                total = 0;
                break;
            }
            else
            {
                total -= nwrite;
                p += nwrite;
            }
        }
        return buflen - total;
    }


    inline ssize_t et_read(int sockfd, std::string &out_put)
    {
        if (sockfd < 0) {
            std::cout << "sockfd is nt 0" << std::endl;
            return -1;
        }
        ssize_t nread  = 0;
        while(true) {
            int n = 0;
            nread = read(sockfd, &out_put[0] + n, 1024);
            if (nread > 0) {
                n += nread;
                continue;
            }
            else if (nread == -1 && errno != EAGAIN) {
                perror("read error");
                return -1;
            }
            else
            {
                break;
            }
        }
        return out_put.length();
    }

    inline std::vector<int> et_accept(int listenfd)
    {
        std::vector<int> fds;
        while(true) {

            struct sockaddr_in clientaddr;
            socklen_t length = 0;
            int fd = accept(listenfd, (sockaddr*)&clientaddr, &length);
            if (fd > 0)
            {
                setnonblocking(fd);
                fds.emplace_back(fd);
                continue;
            }
            else if (fd == -1)
            {
                if (errno != EAGAIN && errno != ECONNABORTED
                    && errno != EPROTO && errno != EINTR)
                {
                    perror("accept failed");
                }
                break;
            }
        }
        return fds;
    }

    inline void reset_one_shot(int epollfd, int fd, bool bone)
    {
        epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;
        if (bone)
        {
            event.events |= EPOLLONESHOT;
        }

        if (-1 == epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event))
        {
            perror("resetoneshot epoll_ctl error!");
        }
    }

    inline int add_fd(int epollfd, int fd, event_t evt, bool oneshot)
    {
        epoll_event ep_evt;
        ep_evt.data.fd = fd;
        ep_evt.events = EPOLLET;

        if (evt & kReadEvent)
        {
            ep_evt.events |= EPOLLIN;
        }

        if (evt & kWriteEvent)
        {
            ep_evt.events |= EPOLLOUT;
        }

        if (oneshot)
        {
            ep_evt.events |= EPOLLONESHOT;
        }

        int r = epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ep_evt);
        if (r != 0)
        {
            if (errno == ENOENT)
            {
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ep_evt) != 0)
                {
                    return -errno;
                }
            }
        }
        return 0;
    }


    inline int delete_fd(int epollfd, int fd)
    {
        epoll_event evt;
        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &evt) != 0) {
            return -errno;
        }
        return 0;
    }


}
#endif //PROACTOR_SOCKET_HELP_H
