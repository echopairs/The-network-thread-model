//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_REACTOR_IMPL_H
#define REACTOR_REACTOR_IMPL_H

#include <map>
#include <mutex>
#include "event_demultiplexer/event_demultiplexer.h"
#include <unit/thread_pool.h>

namespace reactor
{
    class TimeHeap;
    class ReactorImpl
    {
    public:
        ReactorImpl();
        ~ReactorImpl();

        int RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt);
        int UnRegisterHandler(handle_t t);
        void HandleEvents();

    private:
        std::shared_ptr<utils::ThreadPool> thread_pool_;
        std::mutex events_mutex_;
        std::map<handle_t, std::shared_ptr<IEventHandler> > event_handlers_;
        std::shared_ptr<EventDemultiplexer> event_io_ptr_;
        std::shared_ptr<TimeHeap> event_timer_ptr_;
    };
}

#endif //REACTOR_REACTOR_IMPL_H
