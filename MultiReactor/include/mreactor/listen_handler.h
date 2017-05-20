//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_LISTEN_HANDLER_H
#define MREACTOR_LISTEN_HANDLER_H

#include "event_handler.h"
#include <unit/socket_help.h>
#include <mreactor/conn_task.h>
#include "conn_task.h"
#include <mreactor/reactor.h>
#include <cstring>

namespace   mreactor {

    using ParseCb = std::function<void(const std::string &)>;

    class RequestHandler : public IEventHandler
    {
    public:

        RequestHandler(int fd):IEventHandler(fd) {
            parse_cb_ = [this](const std::string msg) {
                std::cout << "recv msg " << msg << std::endl;
                if (msg.size() < 4) {
                    return;
                }

                if(strncasecmp("time", msg.c_str(), 4) == 0) {
                    get_reactor_ptr()->RegisterHandler(get_this_shared_ptr_(), mreactor::kWriteEvent);
                    return;
                } else if(strncasecmp("exit", msg.c_str(), 4) == 0) {
                    std::cout << "recv exit msg" << std::endl;
                } else {
                    std::cout << "Invalid request:" << msg << std::endl;
                }
                close(get_handle());
                get_reactor_ptr()->UnRegisterHandler(this->get_handle());
                set_this_shared_ptr_(nullptr);
                set_this_shared_ptr_(nullptr);
            };
        }

        void HandleRead() override {
            memset(read_buffer_, 0, sizeof(read_buffer_));
            ssize_t nread = 0;
            int sockfd = get_handle();
            while(true) {
                int n = 0;
                nread = read(sockfd, read_buffer_ + n, 1024);
                if (nread > 0) {
                    n += nread;
                }
                if (nread == -1 && errno != EAGAIN) {
                    perror("read error");
                    return;
                } else {
                    break;
                }
            }
            parse_cb_(read_buffer_);
        }

        void HandleWrite() override {
            struct tm *ttime;
            char now[64];
            time_t tt;
            memset(now, 0, 64);
            tt = time(NULL);
            ttime = localtime(&tt);
            strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
            memset(write_buffer_, 0, 1024);
            ssize_t len = sprintf(write_buffer_, "current time: %s\r\n", now);
            std::cout << "begin send" << std::endl;
            len = utils::et_write(get_handle(), write_buffer_, len);
            if (len > 0) {
                get_reactor_ptr()->RegisterHandler(get_this_shared_ptr_(), mreactor::kReadEvent);
                std::cout << "send ok" << std::endl;
            } else {
                std::cout << "send failed" << std::endl;
            }
        }

        void HandleError() override {
            std::cout << "request handle error" << std::endl;
            get_reactor_ptr()->UnRegisterHandler(get_handle());
            close(get_handle());
            set_this_shared_ptr_(nullptr);
            set_reactor_ptr(nullptr);
        }

        void setParse_cb_(const ParseCb &parse_cb_) {
            RequestHandler::parse_cb_ = parse_cb_;
        }
    private:
        ParseCb parse_cb_;
        char write_buffer_[1024];
        char read_buffer_[1024];
    };

    class ListenHandler : public IEventHandler
    {
    public:
        ListenHandler(const std::string &ip_, unsigned short port_) : IEventHandler(0), ip_(ip_), port_(port_) {}

        void HandleRead() override
        {
            while(true)
            {
//                std::cout << "accept new connect" << std::endl;
                struct sockaddr_in clientaddr;
                socklen_t length = 0;
                int fd = accept(get_handle(), (sockaddr*)&clientaddr, &length);
                if (fd > 0)
                {
                    accept_count_++;
                    // here accept success
                    std::cout << "accept connect ok, the fd is: " << fd << " the accept count is: " << accept_count_ << std::endl;
                    utils::setnonblocking(fd);
                    auto handler = std::make_shared<RequestHandler>(fd);
                    handler->set_this_shared_ptr_(handler);
                    mreactor::event_t evt;
                    evt |= mreactor::kReadEvent;
                    auto conn_task = std::make_shared<ConnTask>(handler, evt);
                    _register_cb(conn_task);
                }
                else if(fd == -1)
                {
                    if (errno != EAGAIN && errno != ECONNABORTED
                        && errno != EPROTO && errno != EINTR)
                    {
                        perror("accept failed");
                    }
                    break ;
                }
            }
            get_reactor_ptr()->RegisterHandler(get_this_shared_ptr_(), mreactor::kReadEvent);
        }

        void HandleWrite() override {

        }

        void HandleError() override  {

        }

        bool Start() {
            mreactor::handle_t listenfd;
            listenfd = utils::create_socket();
            utils::bind_socket(listenfd, ip_, port_);
            utils::listen_socket(listenfd);
            set_handle(listenfd);
//            std::cout << "listen fd is: " << get_handle() << std::endl;
        }

        bool Stop() {
            close(get_handle());
        }

        ~ListenHandler() {
            Stop();
        }

        void set_register_cb(const std::function<void(std::shared_ptr<ConnTask>)> &_register_cb) {
            ListenHandler::_register_cb = _register_cb;
        }

    private:
        int accept_count_ = 0;
        std::function<void(std::shared_ptr<ConnTask>)> _register_cb;
        std::string ip_;
        unsigned short port_;
    };
}
#endif //MREACTOR_LISTEN_HANDLER_H
