//
// Created by pairs on 5/18/17.
//

#include <mreactor/event_handler.h>
#include <iostream>
#include "reactor_impl.h"
#include "event_demultiplexer/impl/epoll_demultiplexer.h"

namespace mreactor {

    ReactorImpl::ReactorImpl() : event_io_ptr_(nullptr), event_timer_ptr_(nullptr) {
        event_io_ptr_ = std::make_shared<EpollDemultiplexer>();
        // todo add signal && time_heap
    }

    ReactorImpl::~ReactorImpl() {
    }

    int ReactorImpl::RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt) {
        event_handlers_[handler->get_handle()] = handler;
        return event_io_ptr_->RegisterEvent(handler->get_handle(), evt);
    }


    int ReactorImpl::UnRegisterHandler(handle_t t) {
        event_handlers_.erase(t);
        return event_io_ptr_->UnRegisterEvent(t);
    }

    void ReactorImpl::HandleEvents() {
        // TODO signal events
        // TODO timeout events
        if (event_handlers_.size() <= 0 ) {
            return;
        }
        event_io_ptr_->WaitEvents(&event_handlers_);
    }
}