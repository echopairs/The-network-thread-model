//
// Created by pairs on 4/23/17.
//

#ifndef PROACTOR_PROACTOR_IMPL_H
#define PROACTOR_PROACTOR_IMPL_H

#include <utils/thread_pool.h>
#include <proactor/event_handle.h>
#include <map>
#include "event_demultiplexer/event_demultiplexer.h"

using namespace utils;
namespace proactor
{
    class ProactorImpl
    {
    public:
        ProactorImpl(size_t num = 4);
        ~ProactorImpl();

        int RegisterHandler(std::shared_ptr<EventHandle> handler, event_t evt);

        int UnRegisterHandler(handle_t t);

        void HandleEvents();
    private:
        std::mutex events_mutex_;
        std::map<handle_t, std::shared_ptr<EventHandle> > event_handlers_;
        std::shared_ptr<EventDemultiplexer> event_io_ptr_;
        std::shared_ptr<ThreadPool> thread_pool_;
    };
}
#endif //PROACTOR_PROACTOR_IMPL_H
