//
// Created by pairs on 4/23/17.
//


#include <proactor/event_handle.h>
#include <sys/epoll.h>
#include <cassert>
#include <utils/socket_help.h>
#include "epoll_demultiplexer.h"
#include <vector>

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
        int errcode = utils::add_fd(epfd_, fd, et, false);
        if (!errcode)
        {
            // 0 is ok
            fd_num_++;
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

    void EpollDemultiplexer::WaitEvents(std::map<handle_t, std::shared_ptr<EventHandle> > *handlers)
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
                    thread_pool_->enqueue((*handlers)[fd]->get_err_handle_cb(), 1);
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
                                thread_pool_->enqueue((*handlers)[fd]->get_connect_handle_cb(), fds);
                                continue;
                            }
                        }
                        std::string buffer;
                        auto length = utils::et_read(fd, buffer);
                        if (length == -1) {
                            std::cout << "fd: " << fd << "recv data failed" << std::endl;
                            thread_pool_->enqueue((*handlers)[fd]->get_err_handle_cb(), 1);
                            continue;
                        }
                        else
                        {
                            // read ok
                            thread_pool_->enqueue((*handlers)[fd]->get_read_handle_cb(), buffer);
                        }
                    }

                    if(ep_vets[i].events & EPOLLOUT)
                    {
                        thread_pool_->enqueue((*handlers)[fd]->get_write_handle_cb());
                    }
                }
            }
        }
    }

    EpollDemultiplexer::~EpollDemultiplexer() {
        close(epfd_);
    }


}



