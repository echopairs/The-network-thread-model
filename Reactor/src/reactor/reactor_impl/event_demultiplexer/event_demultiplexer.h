//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_EVENT_DEMULTIPLEXER_H
#define REACTOR_EVENT_DEMULTIPLEXER_H

#include <reactor/event_handler.h>
#include <map>

namespace reactor
{

    class EventDemultiplexer
    {
    public:
        virtual int RegisterEvent(handle_t , event_t) = 0;
        virtual int UnRegisterEvent(handle_t) = 0;

        virtual void WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler>> *handlers) = 0;
    };
}
#endif //REACTOR_EVENT_DEMULTIPLEXER_H
