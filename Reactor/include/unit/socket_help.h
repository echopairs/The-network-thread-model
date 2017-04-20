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
}
#endif //REACTOR_SOCKET_HELP_H
