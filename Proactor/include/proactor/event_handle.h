//
// Created by echopairs on 4/23/17.
//

#ifndef PROACTOR_EVENT_HANDLE_H
#define PROACTOR_EVENT_HANDLE_H

#include <memory>
#include <string>
#include <vector>

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

    class IEventHandler
    {
    public:
        IEventHandler(handle_t fd = 0):fd_(fd), is_listenfd_(false) {}
        virtual void HandleRead() = 0;
        virtual void HandleWrite() = 0;
        virtual void HandleError() = 0;
        handle_t get_handle_fd() {
            return fd_;
        }

        bool get_is_listenfd() const {
            return is_listenfd_;
        }

        void set_is_listenfd(bool is_listenfd_) {
            IEventHandler::is_listenfd_ = is_listenfd_;
        }

        virtual ~IEventHandler(){}

    private:
        bool is_listenfd_;
        handle_t fd_;

    };

//    class EventHandle
//    {
//    public:
//        EventHandle():is_listenfd_(false) {
//        }
//
//        void set_read_handle_cb(ReadCb cb) {
//            this->read_handle_cb_ = cb;
//        }
//
//        std::function<void (const std::string & )> get_read_handle_cb() {
//            return this->read_handle_cb_;
//        }
//
//        void set_write_handle_cb(WriteCb cb) {
//            this->write_handle_cb_ = cb;
//        }
//
//      std::function<void()> get_write_handle_cb() {
//            return this->write_handle_cb_;
//        }
//
//        void set_err_handle_cb(ErrorCb cb) {
//            this->err_handle_cb_ = cb;
//        }
//
//        std::function<void(int errocode)> get_err_handle_cb() {
//            return this->err_handle_cb_;
//        }
//
//
//        int get_handle_fd() const {
//            return handle_fd_;
//        }
//
//        void set_handle_fd(int handle_fd_) {
//            EventHandle::handle_fd_ = handle_fd_;
//        }
//
//        bool get_is_listenfd() const {
//            return is_listenfd_;
//        }
//
//        void set_is_listenfd(bool is_listenfd_) {
//            EventHandle::is_listenfd_ = is_listenfd_;
//        }
//
//        const std::function<void(const std::vector<int> &)> &get_connect_handle_cb() const {
//            return connect_handle_cb_;
//        }
//
//        void set_connect_handle_cb(const std::function<void(const std::vector<int> &)> &connect_handle_cb_) {
//            EventHandle::connect_handle_cb_ = connect_handle_cb_;
//        }
//
//    private:
//        int handle_fd_;
//        bool is_listenfd_;
//
//        std::function<void (const std::vector<int> &fds) > connect_handle_cb_;
//        std::function<void(const std::string &)> read_handle_cb_;
//        std::function<void()> write_handle_cb_;
//        std::function<void(int errocode)> err_handle_cb_;
//    };
}
#endif //PROACTOR_EVENT_HANDLE_H
