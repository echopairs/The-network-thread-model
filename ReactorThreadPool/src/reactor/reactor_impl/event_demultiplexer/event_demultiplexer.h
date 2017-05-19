//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_EVENT_DEMULTIPLEXER_H
#define REACTOR_EVENT_DEMULTIPLEXER_H

#include <reactor/event_handler.h>
#include <map>
#include <unit/thread_pool.h>

namespace reactor
{

    class EventDemultiplexer
    {
    public:
        EventDemultiplexer(std::shared_ptr<utils::ThreadPool> tp):thread_pool_(tp) {}
        virtual int RegisterEvent(handle_t , event_t) = 0;
        virtual int UnRegisterEvent(handle_t) = 0;

        virtual void WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler>> *handlers) = 0;

    protected:
        std::shared_ptr<utils::ThreadPool> thread_pool_;
    };
}
#endif //REACTOR_EVENT_DEMULTIPLEXER_H
