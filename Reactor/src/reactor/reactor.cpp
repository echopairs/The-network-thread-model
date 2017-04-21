//
// Created by pairs on 4/18/17.
//

#include <reactor/reactor.h>
#include "reactor_impl/reactor_impl.h"

namespace reactor
{


    Reactor::Reactor():reactor_impl_ptr_(nullptr)
    {
        reactor_impl_ptr_ =  std::make_shared<ReactorImpl>();
    }

    Reactor::~Reactor()
    {

    }

    int Reactor::RegisterHandler(std::shared_ptr <IEventHandler> handler, event_t evt)
    {
        reactor_impl_ptr_->RegisterHandler(handler, evt);
    }

    int Reactor::UnRegisterHandler(handle_t t)
    {
        reactor_impl_ptr_->UnRegisterHandler(t);
    }

    int Reactor::RegisterTimerTask()
    {
        return 0;
    }

    void Reactor::HandleEvents()
    {
        reactor_impl_ptr_->HandleEvents();
    }
}