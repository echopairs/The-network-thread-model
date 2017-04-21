//
// Created by echopairs on 4/20/17.
//

#ifndef REACTOR_REQUEST_HANDLER_H
#define REACTOR_REQUEST_HANDLER_H

#include <reactor/event_handler.h>
#include <unit/socket_help.h>
#include <sys/epoll.h>
#include <unit/singleton.h>
#include <reactor/reactor.h>
#include <unistd.h>
#include <iostream>

namespace reactor
{
    using  ParseCb = std::function<void(const std::string &)>;

    class RequestHandler : public IEventHandler
    {
    public:

        RequestHandler(handle_t fd):IEventHandler(fd)
        {
            parse_cb_ = [this](const std::string msg) {
                std::cout << msg << std::endl;
                if (msg.size() < 4) {
                    return;
                }

                if (strncasecmp("time", msg.c_str(), 4) == 0)
                {
                    unit::SingleTon<Reactor>::Instance()->RegisterHandler(get_this_shared_ptr_(), reactor::kWriteEvent);
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
                close(get_handle());
                unit::SingleTon<Reactor>::Instance()->UnRegisterHandler(this->get_handle());
            };
        }
        virtual void HandleRead()
        {
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
                }
                else
                {
                    break;
                }
            }
            parse_cb_(read_buffer_);
        }
        virtual void HandleWrite() {
            struct tm *ttime;
            char now[64];
            time_t tt;
            memset(now, 0, 64);
            tt = time(NULL);
            ttime = localtime(&tt);
            strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
            memset(write_buffer_, 0, 1024);
            ssize_t len = sprintf(write_buffer_, "current time: %s\r\n", now);
            len = reactor::socket_write(get_handle(), write_buffer_, len);
            if (len > 0) {
                unit::SingleTon<Reactor>::Instance()->RegisterHandler(get_this_shared_ptr_(), reactor::kWriteEvent);
            }
            else {
                std::cout << "send failed" << std::endl;
            }
        }
        virtual void HandleError() {
            close(get_handle());
            unit::SingleTon<Reactor>::Instance()->UnRegisterHandler(get_handle());
        }

        void setParse_cb_(const ParseCb &parse_cb_) {
            RequestHandler::parse_cb_ = parse_cb_;
        }

    private:
        ParseCb parse_cb_;
        char write_buffer_[1024];
        char read_buffer_[1024];
    };


    class ListenServer : public IEventHandler
    {

    public:
        ListenServer(const std::string &ip_, unsigned short port_) : IEventHandler(0), ip_(ip_), port_(port_) {}

        bool Start()
        {
            reactor::handle_t listenfd;

            listenfd = reactor::create_socket();
            reactor::bind_socket(listenfd, port_);
            reactor::listen_socket(listenfd);
            set_handle(listenfd);

            std::cout << "listen handle id is :" << listenfd << std::endl;
        }

        virtual void HandleRead() {
            std::cout << "recv connect" << std::endl;
            while(true)
            {
                struct sockaddr_in clientaddr;
                socklen_t length = 0;
                int fd = accept(get_handle(), (sockaddr*)&clientaddr, &length);
                if (fd > 0)
                {
                    // here accept success
                    setnonblocking(fd);
                    auto handler = std::make_shared<RequestHandler>(fd);
                    handler->set_this_shared_ptr_(handler);
                    auto reactor = unit::SingleTon<Reactor>::Instance();

                    reactor::event_t  evt;
                    evt |= reactor::kReadEvent;
                    reactor->RegisterHandler(handler, evt);
                }
                else if (fd == -1)
                {
                    if (errno != EAGAIN && errno != ECONNABORTED
                        && errno != EPROTO && errno != EINTR)
                    {
                        perror("accept failed");
                    }
                    return ;
                }
            }
        }

        virtual void HandleWrite() {

        }

        virtual void HandleError() {

        }


    private:
        std::string ip_;
        unsigned short port_;

    };

}   // namespace reactor

#endif //REACTOR_REQUEST_HANDLER_H
