//
// Created by pairs on 4/18/17.
//

#include <reactor/event_handler.h>
#include <iostream>
#include "reactor_impl.h"
#include "event_demultiplexer/impl/epoll_demultiplexer.h"

namespace reactor
{

    ReactorImpl::ReactorImpl():event_io_ptr_(nullptr),event_timer_ptr_(nullptr)
    {
        event_io_ptr_ = std::make_shared<EpollDemultiplexer>();
        // todo add signal && time_heap
    }

    ReactorImpl::~ReactorImpl()
    {
    }

    int ReactorImpl::RegisterHandler(std::shared_ptr <IEventHandler> handler, event_t evt)
    {
        // just one thread not necessity but we may change to multithreading
        std::lock_guard<std::mutex> l(events_mutex_);
        std::cout << "Register handle, handle id is: " << handler->get_handle() << std::endl;
        event_handlers_[handler->get_handle()] = handler;
        return event_io_ptr_->RegisterEvent(handler->get_handle(), evt);
    }


    int ReactorImpl::UnRegisterHandler(handle_t t)
    {
        std::lock_guard<std::mutex> l(events_mutex_);
        event_handlers_.erase(t);
        return event_io_ptr_->UnRegisterEvent(t);
    }

    void ReactorImpl::HandleEvents()
    {
        // TODO signal events
        // TODO timeout events
        std::cout << "tick task for handleEvents" << std::endl;
        event_io_ptr_->WaitEvents(&event_handlers_);
    }
}

