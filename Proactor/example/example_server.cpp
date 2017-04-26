//
// Created by echopairs on 4/25/17.
//

#include "listen_server.h"

#include <proactor/proactor.h>
#include <thread>
#include "listen_server.h"

using namespace proactor;
int main()
{
    auto server = std::make_shared<ListenHandler>("127.0.0.1", 23333);
    server->Start();
    server->set_this_shared_ptr_(server);
    utils::SingleTon<Proactor>::Instance()->RegisterHandler(server, proactor::kReadEvent);
    while(1) {
        utils::SingleTon<Proactor>::Instance()->HandleEvents();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}