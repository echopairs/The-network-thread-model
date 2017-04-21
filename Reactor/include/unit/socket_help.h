//
// Created by echopairs on 4/20/17.
//

#ifndef REACTOR_SOCKET_HELP_H
#define REACTOR_SOCKET_HELP_H

#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include <thread>
#include <iostream>

namespace reactor {

   inline void setnonblocking(int sock)
    {
        int opts;
        opts = fcntl(sock, F_GETFL);

        if (opts < 0)
        {
            perror("fcntl(sock, GETFL)");
            exit(1);
        }

        opts = opts | O_NONBLOCK;

        if (fcntl(sock, F_SETFL, opts) < 0)
        {
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

    inline int bind_socket(int sockfd, unsigned int port)
    {
        struct sockaddr_in serveraddr;
        bzero(&serveraddr, sizeof(serveraddr));

        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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


    ssize_t socket_write(int sockfd, const char* buffer, size_t buflen)
    {
        size_t total = buflen;
        const char* p = buffer;
        ssize_t  nwrite = 0;

        while(total)
        {
            nwrite = write(sockfd, p, total); // ET so we need write and write
            if (nwrite < 0)
            {
                // 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满
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
}
#endif //REACTOR_SOCKET_HELP_H
