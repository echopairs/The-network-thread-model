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

    ssize_t et_write(int sockfd, const char* buffer, size_t buflen)
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


    ssize_t et_read(int sockfd, std::string &out_put)
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

    std::vector<int> et_accept(int listenfd)
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
}
#endif //PROACTOR_SOCKET_HELP_H
