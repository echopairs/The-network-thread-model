//
// Created by pairs on 4/23/17.
//


#include <proactor/event_handle.h>
#include <sys/epoll.h>
#include <cassert>
#include <utils/socket_help.h>
#include "epoll_demultiplexer.h"
#include <vector>
#include <cstring>

namespace proactor
{
    EpollDemultiplexer::EpollDemultiplexer(std::shared_ptr<utils::ThreadPool> tp):EventDemultiplexer(tp)
    {
        epfd_ = epoll_create(FD_SETSIZE);
        assert(epfd_ != -1);
        fd_num_ = 0;
    }

    int EpollDemultiplexer::RegisterEvent(handle_t fd, event_t et)
    {
        et |= proactor::kReadEvent;
        int errcode = utils::add_fd(epfd_, fd, et, false);
        if (errcode < 0) {
            perror("add fd to epoll failed");
            return errcode;
        }
        else if (errcode == 1) {
            // add ok
            fd_num_++;
        }
        else {
            // mod ok
        }
        return 0;
    }

    int EpollDemultiplexer::UnRegisterEvent(handle_t fd) {
        int errcode = utils::delete_fd(epfd_, fd);
        if (!errcode)
        {
            // delete ok
            fd_num_--;
        }
        return 0;
    }

    void EpollDemultiplexer::WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler> > *handlers)
    {
        std::vector<epoll_event> ep_vets(fd_num_);
        int num = epoll_wait(epfd_, &ep_vets[0], ep_vets.size(), -1);
        if (num > 0)
        {
            for (size_t i = 0; i < num; i++)
            {
                handle_t fd = ep_vets[i].data.fd;
                assert(handlers->find(fd) != handlers->end());

                if (ep_vets[i].events & EPOLLERR ||
                        ep_vets[i].events & EPOLLHUP)
                {
                    auto error_task = std::bind(&IEventHandler::HandleError, (*handlers)[fd], std::placeholders::_1);
                    thread_pool_->enqueue(error_task, 1);
                    continue;
                }
                else
                {
                    if (ep_vets[i].events & EPOLLIN)
                    {
                       bool is_listenfd = (*handlers)[fd]->get_is_listenfd();
                        if (is_listenfd)
                        {
                            auto fds = utils::et_accept(fd);
                            if (fds.size() > 0 )
                            {
                                auto conn_task = std::bind(&IEventHandler::HandleConnect, (*handlers)[fd], fds);
                                thread_pool_->enqueue(conn_task);
                                continue;
                            }
                        }
                        char buffer[1024];
                        memset((void*)buffer, 0, 1024);
                        auto length = utils::et_read(fd, buffer);
                        if (length == -1) {
                            std::cout << "fd: " << fd << "recv data failed" << std::endl;
                            auto error_task = std::bind(&IEventHandler::HandleError, (*handlers)[fd], std::placeholders::_1);
                            thread_pool_->enqueue(error_task, 1);
                            continue;
                        }
                        else
                        {
                            // read ok
                            auto read_task = std::bind(&IEventHandler::HandleRead, (*handlers)[fd], std::placeholders::_1);
                            thread_pool_->enqueue(read_task, buffer);
                        }
                    }

                    if(ep_vets[i].events & EPOLLOUT)
                    {
                        auto write_task = std::bind(&IEventHandler::HandleWrite, (*handlers)[fd]);
                        thread_pool_->enqueue(write_task);
                    }
                }
            }
        }
    }

    EpollDemultiplexer::~EpollDemultiplexer() {
        close(epfd_);
    }


}



