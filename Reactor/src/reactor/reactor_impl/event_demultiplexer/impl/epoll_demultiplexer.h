//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_EPOLL_DEMULTIPLEXER_H
#define REACTOR_EPOLL_DEMULTIPLEXER_H

#include "../event_demultiplexer.h"

namespace reactor
{
    class EpollDemultiplexer : public EventDemultiplexer
    {
    public:
        EpollDemultiplexer();
        ~EpollDemultiplexer();

        int RegisterEvent(handle_t , event_t) override ;
        int UnRegisterEvent(handle_t) override ;

        void WaitEvent() override ;
    };
}
#endif //REACTOR_EPOLL_DEMULTIPLEXER_H
