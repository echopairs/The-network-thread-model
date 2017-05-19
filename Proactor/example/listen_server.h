//
// Created by echopairs on 4/25/17.
//

#ifndef PROACTOR_LISTEN_SERVER_H
#define PROACTOR_LISTEN_SERVER_H

#include <proactor/event_handle.h>
#include <proactor/proactor.h>
#include <utils/socket_help.h>
#include <utils/singleton.h>
#include <string.h>
#include <atomic>

namespace proactor
{

    class RequestHandler :public IEventHandler
    {
    public:
        RequestHandler(handle_t fd):IEventHandler(fd) {
            count_ = 0;
        }
        virtual void HandleWrite() {
            std::cout << "do handle write" << std::endl;
            struct tm *ttime;
            char now[64];
            time_t tt;
            memset(now, 0, 64);
            tt = time(NULL);
            ttime = localtime(&tt);
            strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
            memset(write_buffer_, 0, 1024);
            ssize_t len = sprintf(write_buffer_, "current time: %s\r\n", now);
            len = utils::et_write(get_handle_fd(), write_buffer_, len);
            if (len > 0) {
                std::cout << "write ok" << std::endl;
               // utils::SingleTon<Proactor>::Instance()->RegisterHandler(get_this_shared_ptr_(), proactor::kReadEvent);
            }
            else {
                std::cout << "send failed" << std::endl;
            }
        }

        virtual void HandleRead(const std::string & msg) {
            count_++;
            std::cout << "do handle read msg is: " << msg << std::endl;
            if (msg.size() < 4) {
                return;
            }

            if (strncasecmp("time", msg.c_str(), 4) == 0)
            {
                utils::SingleTon<Proactor>::Instance()->RegisterHandler(get_this_shared_ptr_(), proactor::kWriteEvent);
                return;
            }
            else if(strncasecmp("exit", msg.c_str(), 4) == 0)
            {
                std::cout << "recv exit msg" << std::endl;
            }
            else
            {
                std::cout << "Invalid request: " << msg << std::endl;
            }
            close(get_handle_fd());
            utils::SingleTon<Proactor>::Instance()->UnRegisterHandler(this->get_handle_fd());
        }

        virtual void HandleError(int errorcode) {
            std::cout << "do handleError" << std::endl;
            std::cout << "the counter is:" << count_ << std::endl;
            utils::SingleTon<Proactor>::Instance()->UnRegisterHandler(this->get_handle_fd());
            set_this_shared_ptr_(nullptr);
            close(get_handle_fd());
        }

    private:
        char write_buffer_[1024];
        std::atomic<int> count_;
    };
   // std::atomic<int> RequestHandler::count_ = 0;

    class ListenHandler :public IEventHandler
    {
    public:
        ListenHandler(const std::string &ip, unsigned int port):IEventHandler(0), ip_(ip),port_(port) {
            set_is_listenfd(true);
        }

        virtual void HandleWrite() {
            return;
        }

        virtual void HandleRead(const std::string & msg) {
            return;
        }

        virtual void HandleError(int errorcode) {
            set_this_shared_ptr_(nullptr);
            close(get_handle_fd());
        }

        virtual void HandleConnect (const std::vector<int> &fds) {
            for (auto fd: fds) {
                std::cout << "the request fd is: " << fd << std::endl;
                auto handler = std::make_shared<RequestHandler>(fd);
                handler->set_this_shared_ptr_(handler);
                proactor::event_t evt;
                evt |= proactor::kReadEvent;
                utils::SingleTon<Proactor>::Instance()->RegisterHandler(handler, evt);
            }
        }

        bool Start()
        {
            proactor::handle_t listenfd;
            listenfd = utils::create_socket();
            utils::bind_socket(listenfd, ip_, port_);
            utils::listen_socket(listenfd);
            set_handle_fd(listenfd);
        }

        ~ListenHandler() {
            close(get_handle_fd());
        }

    private:
        std::string ip_;
        unsigned int port_;
    };
}   // namespace proactor
#endif //PROACTOR_LISTEN_SERVER_H
