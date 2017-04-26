//
// Created by pairs on 4/23/17.
//

#ifndef PROACTOR_EPOLL_DEMULTIPLEXER_H
#define PROACTOR_EPOLL_DEMULTIPLEXER_H

#include <atomic>
#include "../event_demultiplexer.h"

namespace proactor
{
    class EpollDemultiplexer : public EventDemultiplexer
    {
    public:
        EpollDemultiplexer(std::shared_ptr<utils::ThreadPool> tp);
        int RegisterEvent(handle_t, event_t) override ;
        int UnRegisterEvent(handle_t) override ;
        ~EpollDemultiplexer();
        void WaitEvents(std::map<handle_t,std::shared_ptr<IEventHandler> > *handler) override ;

        const std::atomic<int> &get_fd_num() const {
            return fd_num_;
        }

        void set_fd_num_(int fd_num) {
            fd_num_ =  fd_num;
        }

    private:
        int epfd_;
        std::atomic<int> fd_num_;
    };
}   // namespace proactor
#endif //PROACTOR_EPOLL_DEMULTIPLEXER_H
