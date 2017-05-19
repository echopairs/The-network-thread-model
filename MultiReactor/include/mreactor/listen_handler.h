//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_LISTEN_HANDLER_H
#define MREACTOR_LISTEN_HANDLER_H

#include "event_handler.h"
#include <unit/socket_help.h>
#include <mreactor/conn_task.h>
#include "conn_task.h"

namespace   mreactor {

    class RequestHandler : public IEventHandler
    {
    public:

        RequestHandler(int fd):IEventHandler(fd) {

        }

        void HandleRead() override {

        }

        void HandleWrite() override {

        }

        void HandleError() override {

        }
    };

    class ListenHandler : public IEventHandler
    {
    public:
        ListenHandler(const std::string &ip_, unsigned short port_) : IEventHandler(0), ip_(ip_), port_(port_) {}

        void HandleRead() override
        {
            while(true)
            {
                struct sockaddr_in clientaddr;
                socklen_t length = 0;
                int fd = accept(get_handle(), (sockaddr*)&clientaddr, &length);
                if (fd > 0)
                {
                    // here accept success
                    utils::setnonblocking(fd);
                    auto handler = std::make_shared<RequestHandler>(fd);
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
        std::function<void(std::shared_ptr<ConnTask>)> _register_cb;
        std::string ip_;
        unsigned short port_;
    };
}
#endif //MREACTOR_LISTEN_HANDLER_H
