//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_EVENT_HANDLER_H
#define REACTOR_EVENT_HANDLER_H

#include <memory>

namespace reactor
{
using handle_t = int;
using event_t = unsigned int;
    enum
    {
        kReadEvent = 0x01,
        kWriteEvent = 0x02,
        kErrorEvent = 0x04,
        kEventMask = 0xff
    };

class IEventHandler
{
public:
    IEventHandler(handle_t fd):fd_(fd) {}
    virtual void HandleRead() = 0;
    virtual void HandleWrite() = 0;
    virtual void HandleError() = 0;
    handle_t get_handle() { return fd_; };
protected:
    handle_t fd_;
    IEventHandler(){}
    virtual ~IEventHandler(){}
};
}   // namespace reactor
#endif //REACTOR_EVENT_HANDLER_H
