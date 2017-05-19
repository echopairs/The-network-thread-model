//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_EPOLL_DEMULTIPLEXER_H
#define MREACTOR_EPOLL_DEMULTIPLEXER_H

#include "../event_demultiplexer.h"
#include <atomic>

namespace mreactor {

    class EpollDemultiplexer : public EventDemultiplexer {
    public:
        EpollDemultiplexer();

        ~EpollDemultiplexer();

        int RegisterEvent(handle_t, event_t) override;

        int UnRegisterEvent(handle_t) override;

        void WaitEvents(std::map<handle_t, std::shared_ptr<IEventHandler>> *handlers) override;

    private:
        int epfd_;
        std::atomic<int> fd_num_;
    };
}
#endif //MREACTOR_EPOLL_DEMULTIPLEXER_H
