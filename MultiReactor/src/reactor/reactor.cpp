//
// Created by pairs on 5/18/17.
//

#include <mreactor/reactor.h>
#include <thread>
#include <iostream>
#include "reactor_impl/reactor_impl.h"

namespace mreactor  {

    Reactor::Reactor():_reactor_impl_ptr(nullptr),_if_stop(false) {
        _reactor_impl_ptr = std::make_shared<ReactorImpl>();
    }

    Reactor::~Reactor() {
    }

    int Reactor::RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt) {
        _reactor_impl_ptr->RegisterHandler(handler, evt);
    }

    int Reactor::UnRegisterHandler(handle_t h) {
        _reactor_impl_ptr->UnRegisterHandler(h);
    }

    int Reactor::RegisterTimerTask() {
        // TODO
    }

    void Reactor::HandleEvents(std::function<void()> cb) {
        while(!_if_stop) {
            _reactor_impl_ptr->HandleEvents();
            if(cb) {
                cb();
            }
        //    std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }

}   // namespace mreactor