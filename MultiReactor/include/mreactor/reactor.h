//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_REACTOR_H
#define MREACTOR_REACTOR_H

#include <atomic>
#include "event_handler.h"

namespace mreactor {

    class ReactorImpl;

    class Reactor
    {
    public:
        Reactor();
        ~Reactor();
        int RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt);
        int UnRegisterHandler(handle_t h);

        void HandleEvents(std::function<void()>cb = nullptr);
        int RegisterTimerTask();

    private:
        std::atomic<bool> _if_stop;

        Reactor(const Reactor &);
        Reactor &operator = (const Reactor &);
        std::shared_ptr<ReactorImpl> _reactor_impl_ptr;
    };
}   // namespace mreactor
#endif //MREACTOR_REACTOR_H
