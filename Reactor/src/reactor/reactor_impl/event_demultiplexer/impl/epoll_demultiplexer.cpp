//
// Created by echopairs on 4/19/17.
//

#include "epoll_demultiplexer.h"
#include <sys/epoll.h>
#include <assert.h>
#include <zconf.h>
#include <vector>
#include <iostream>

namespace reactor
{
    EpollDemultiplexer::EpollDemultiplexer()
    {
        epfd_ = epoll_create(FD_SETSIZE);
        assert(epfd_ != -1);
        fd_num_ = 0;
    }

    int EpollDemultiplexer::RegisterEvent(handle_t fd, event_t evt)
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

        ep_evt.events |= EPOLLONESHOT;

        int r = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ep_evt);
        if (r != 0)
        {
            if (errno == ENOENT)
            {
                if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ep_evt) != 0)
                {
                    return -errno;
                }
            }
        }
        fd_num_++;
        return 0;
    }

    EpollDemultiplexer::~EpollDemultiplexer()
    {
        close(epfd_);
    }

    int EpollDemultiplexer::UnRegisterEvent(handle_t fd)
    {
        epoll_event evt;
        if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, &evt) != 0) {
            return -errno;
        }
        --fd_num_;
        return 0;
    }

    void EpollDemultiplexer::WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler>> *handlers)
    {
        std::vector<epoll_event> ep_vets(fd_num_);
        std::cout << "begin epoll_wait: " << fd_num_ << std::endl;
        int num = epoll_wait(epfd_, &ep_vets[0], ep_vets.size(), -1);
        std::cout << "after epoll_wait" << num << std::endl;
        if (num > 0)
        {
            for (size_t i = 0; i < num; i++)
            {
                handle_t fd = ep_vets[i].data.fd;
                assert(handlers->find(fd) != handlers->end());

                if (ep_vets[i].events & EPOLLERR ||
                        ep_vets[i].events & EPOLLHUP)
                {
                    (*handlers)[fd]->HandleError();
                }
                else
                {
                    if (ep_vets[i].events & EPOLLIN)
                    {
                        (*handlers)[fd]->HandleRead();
                    }
                    if (ep_vets[i].events & EPOLLOUT)
                    {
                        (*handlers)[fd]->HandleWrite();
                    }
                }
            }
        }
    }


}