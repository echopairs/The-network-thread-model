//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_EPOLL_DEMULTIPLEXER_H
#define REACTOR_EPOLL_DEMULTIPLEXER_H

#include "../event_demultiplexer.h"
#include <atomic>
#include <unit/thread_pool.h>

namespace reactor
{

    class EpollDemultiplexer : public EventDemultiplexer
    {
    public:
        EpollDemultiplexer(std::shared_ptr<utils::ThreadPool>);
        ~EpollDemultiplexer();

        int RegisterEvent(handle_t , event_t) override ;
        int UnRegisterEvent(handle_t) override ;

        void WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler>> *handlers) override ;

    private:
        int epfd_;
        std::atomic<int> fd_num_;
    };
}
#endif //REACTOR_EPOLL_DEMULTIPLEXER_H
