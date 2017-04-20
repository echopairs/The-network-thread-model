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

namespace reactor
{
    class RequestHandler : public IEventHandler
    {
    public:
        RequestHandler(handle_t fd):IEventHandler(fd) {

        }
        virtual void HandleRead() {

        }
        virtual void HandleWrite() {

        }
        virtual void HandleError() {

        }
    };


    class ListenServer : public IEventHandler
    {

    public:
        ListenServer(const std::string &ip_, unsigned short port_) : ip_(ip_), port_(port_) {}

        bool Start()
        {
            handle_ = reactor::create_socket();
            reactor::bind_socket(handle_, 23333);
            reactor::listen_socket(handle_);
        }

        virtual void HandleRead() {
            while(true)
            {
                struct sockaddr_in clientaddr;
                socklen_t length = 0;
                int fd = accept(handle_, (sockaddr*)&clientaddr, &length);
                if (fd > 0)
                {
                    // here accept success
                    setnonblocking(fd);
                    auto handler = std::make_shared<RequestHandler>(fd);
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
        reactor::handle_t handle_;
        std::string ip_;
        unsigned short port_;

    };

}   // namespace reactor

#endif //REACTOR_REQUEST_HANDLER_H
