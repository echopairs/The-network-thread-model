//
// Created by pairs on 4/23/17.
//


#include <cstddef>
#include "proactor_impl.h"
#include "event_demultiplexer/impl/epoll_demultiplexer.h"

namespace proactor {

    ProactorImpl::ProactorImpl(size_t num)
    {
        thread_pool_ = std::make_shared<ThreadPool>(num);
        event_io_ptr_ = std::make_shared<EpollDemultiplexer>(thread_pool_);
        // TODO add singal && time_heap
    }

    int ProactorImpl::RegisterHandler(std::shared_ptr<EventHandle> handler, event_t evt)
    {
        std::lock_guard<std::mutex> lk(events_mutex_);
        event_handlers_[handler->get_handle_fd()] = handler;
        return event_io_ptr_->RegisterEvent(handler->get_handle_fd(), evt);
    }

    int ProactorImpl::UnRegisterHandler(handle_t t)
    {
        std::lock_guard<std::mutex> l(events_mutex_);
        event_handlers_.erase(t);
        return event_io_ptr_->UnRegisterEvent(t);
    }

    void ProactorImpl::HandleEvents()
    {
        // TODO signal events
        // TODO timeout events
        event_io_ptr_->WaitEvents(&event_handlers_);
    }

    ProactorImpl::~ProactorImpl()
    {
        // TODO
    }
}

