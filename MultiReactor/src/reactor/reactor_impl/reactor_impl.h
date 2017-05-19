//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_REACTOR_IMPL_H
#define MREACTOR_REACTOR_IMPL_H
#include <map>
#include <mutex>
#include "event_demultiplexer/event_demultiplexer.h"

namespace mreactor
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
        std::map<handle_t, std::shared_ptr<IEventHandler> > event_handlers_;
        std::shared_ptr<EventDemultiplexer> event_io_ptr_;
        std::shared_ptr<TimeHeap> event_timer_ptr_;
    };
}

#endif //MREACTOR_REACTOR_IMPL_H
