//
// Created by echopairs on 4/25/17.
//

#ifndef PROACTOR_LISTEN_SERVER_H
#define PROACTOR_LISTEN_SERVER_H

#include <proactor/event_handle.h>
#include <proactor/proactor.h>
#include <utils/socket_help.h>
#include <utils/singleton.h>

namespace proactor
{
    class ListenServer
    {
    public:
        ListenServer(const std::string &ip, unsigned int port): ip_(ip),port_(port) {
            event_handle_ = std::make_shared<EventHandle>();
            event_handle_->set_is_listenfd(true);
        }
        bool Start()
        {
            proactor::handle_t listenfd;
            listenfd = utils::create_socket();
            utils::listen_socket(listenfd);

            event_handle_->set_handle_fd(listenfd);
            /// 1. set cb function
            event_handle_->set_connect_handle_cb([this](const std::vector<int>& fds) {

               for (auto fd: fds) {
                   /// 1. create handle
                   auto handler = std::make_shared<EventHandle>();

                   /// 2. set config
                   handler->set_handle_fd(fd);
                   handler->set_read_handle_cb([](const std::string &data) {
                       if (data.size() < 4) {
                           return;
                       }
                       if (strncasecmp("time", data.c_str(), 4) == 0)
                       {

                       }
                       else if(strncasecmp("exit", data.c_str(), 4) == 0)
                       {
                           std::cout << "recv exit msg" << std::endl;
                       }
                       else
                       {
                           std::cout << "Invalid request: " << data << std::endl;
                       }
                   });
//                    handler->set_write_handle_cb();
//                    handler->set_err_handle_cb();

                   /// 3. register to proactor
                   proactor::event_t evt;
                   evt |= proactor::kReadEvent;
                   utils::SingleTon<Proactor>::Instance()->RegisterHandler(handler, evt);
               }
            });

            /// 2. register to proactor
            utils::SingleTon<proactor::Proactor>::Instance()->RegisterHandler(event_handle_, proactor::kReadEvent);
        }
        ~ListenServer() {
        }

    private:
        std::shared_ptr<EventHandle> event_handle_;
        std::string ip_;
        unsigned int port_;
    };
}   // namespace proactor
#endif //PROACTOR_LISTEN_SERVER_H
