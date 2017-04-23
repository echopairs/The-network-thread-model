//
// Created by echopairs on 4/23/17.
//

#ifndef PROACTOR_EVENT_HANDLE_H
#define PROACTOR_EVENT_HANDLE_H

#include <memory>
#include <string>

namespace proactor {

    using handle_t = int;
    using event_t = unsigned int;
    enum
    {
        kReadEvent = 0x01,
        kWriteEvent = 0x02,
        kErrorEvent = 0x04,
        kEventMask = 0xff
    };

    using ReadCb = void(const std::string & );
    using WriteCb = void();
    using ErrorCb = void(int errorcode);

    class EventHandle
    {
    public:
        EventHandle() {}

        void set_read_handle_cb(ReadCb cb) {
            this->read_handle_cb_ = cb;
        }

//        ReadCb get_read_handle_cb() {
//            return this->read_handle_cb_;
//        }

        void set_write_handle_cb(WriteCb cb) {
            this->write_handle_cb_ = cb;
        }

//        WriteCb get_write_handle_cb() {
//            return this->write_handle_cb_;
//        }

        void set_err_handle_cb(ErrorCb cb) {
            this->err_handle_cb_ = cb;
        }

//        ErrorCb get_err_handle_cb() {
//            return this->err_handle_cb_;
//        }

    private:
        std::function<void(const std::string &)> read_handle_cb_;
        std::function<void()> write_handle_cb_;
        std::function<void(int errocode)> err_handle_cb_;
    };
}
#endif //PROACTOR_EVENT_HANDLE_H
