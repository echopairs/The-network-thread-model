//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_EVENT_HANDLER_H
#define MREACTOR_EVENT_HANDLER_H

#include <memory>

namespace mreactor
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
        IEventHandler(handle_t fd = 0):fd_(fd) {}
        virtual void HandleRead() = 0;
        virtual void HandleWrite() = 0;
        virtual void HandleError() = 0;
        handle_t get_handle() { return fd_; };
        handle_t set_handle(handle_t fd) { this->fd_ = fd; };
        const std::shared_ptr<IEventHandler> &get_this_shared_ptr_() const {
            return this_shared_ptr_;
        }

        void set_this_shared_ptr_(const std::shared_ptr<IEventHandler> &this_shared_ptr_) {
            IEventHandler::this_shared_ptr_ = this_shared_ptr_;
        }

    public:
        std::shared_ptr<IEventHandler> this_shared_ptr_;
        handle_t fd_;
        IEventHandler(){}
        virtual ~IEventHandler(){}
    };
}   // namespace reactor
#endif //MREACTOR_EVENT_HANDLER_H
