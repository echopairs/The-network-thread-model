//
// Created by pairs on 5/18/17.
//

#include <mreactor/reactor.h>
#include "reactor_impl/reactor_impl.h"

namespace mreactor  {

    Reactor::Reactor():_reactor_impl_ptr(nullptr),_if_stop(false) {
      //  _reactor_impl_ptr = std::make_shared<ReactorImpl>();
    }

    Reactor::~Reactor() {
        // TODO
    }

    int Reactor::RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt) {
        // todo
    }

    int Reactor::UnRegisterHandler(handle_t h) {
        // todo
    }

    int Reactor::RegisterTimerTask() {
        // TODO
    }

    void Reactor::HandleEvents(std::function<void()> cb) {
        while(!_if_stop) {
            // loop
            _reactor_impl_ptr->HandleEvents();
            if(cb) {
                cb();
            }
        }
    }

}   // namespace mreactor