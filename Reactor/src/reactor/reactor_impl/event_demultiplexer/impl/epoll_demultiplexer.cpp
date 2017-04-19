//
// Created by echopairs on 4/19/17.
//

#include "epoll_demultiplexer.h"

namespace reactor
{

    EpollDemultiplexer::EpollDemultiplexer() {

    }

    int EpollDemultiplexer::RegisterEvent(handle_t, event_t)
    {

    }

    EpollDemultiplexer::~EpollDemultiplexer() {

    }

    int EpollDemultiplexer::UnRegisterEvent(handle_t) {
        return 0;
    }

    void EpollDemultiplexer::WaitEvent() {

    }
}