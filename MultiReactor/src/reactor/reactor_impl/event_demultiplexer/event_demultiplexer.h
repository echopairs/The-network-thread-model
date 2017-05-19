//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_EVENT_DEMULTIPLEXER_H
#define MREACTOR_EVENT_DEMULTIPLEXER_H

#include <mreactor/event_handler.h>
#include <map>

namespace mreactor
{

    class EventDemultiplexer
    {
    public:
        virtual int RegisterEvent(handle_t , event_t) = 0;
        virtual int UnRegisterEvent(handle_t) = 0;

        virtual void WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler>> *handlers) = 0;
    };
}
#endif //MREACTOR_EVENT_DEMULTIPLEXER_H
