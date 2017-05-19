//
// Created by pairs on 5/19/17.
//

#ifndef MREACTOR_CONN_TASK_H
#define MREACTOR_CONN_TASK_H

#include "event_handler.h"

namespace mreactor {

    class ConnTask
    {
    public:
        ConnTask(std::shared_ptr<IEventHandler> ev_ptr, event_t t):_event_ptr(ev_ptr),_evt_type(t) {

        }

        event_t get_evt_type() const {
            return _evt_type;
        }

        const std::shared_ptr<IEventHandler> &get_event_ptr() const {
            return _event_ptr;
        }

    private:
        event_t _evt_type;
        std::shared_ptr<IEventHandler> _event_ptr;
    };
}   // namespace mreactor
#endif //MREACTOR_CONN_TASK_H
