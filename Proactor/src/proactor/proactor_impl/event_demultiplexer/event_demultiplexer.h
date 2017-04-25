//
// Created by pairs on 4/23/17.
//

#ifndef PROACTOR_EVENT_DEMULTIPLEXER_H
#define PROACTOR_EVENT_DEMULTIPLEXER_H

#include <memory>
#include <proactor/event_handle.h>
#include <map>
#include <utils/thread_pool.h>

namespace proactor
{
    class EventDemultiplexer
    {
    public:

        EventDemultiplexer(std::shared_ptr<utils::ThreadPool> tp):thread_pool_(tp) {
        }
        virtual int RegisterEvent(handle_t, event_t) = 0;
        virtual int UnRegisterEvent(handle_t) = 0;

        virtual void WaitEvents(std::map<handle_t,std::shared_ptr<EventHandle> > *handler) = 0;

    protected:
        std::shared_ptr<utils::ThreadPool> thread_pool_;
    };
}
#endif //PROACTOR_EVENT_DEMULTIPLEXER_H
