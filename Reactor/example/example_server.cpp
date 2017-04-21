//
// Created by echopairs on 4/20/17.
//

#include <reactor/reactor.h>
#include "request_handler.h"

using namespace reactor;
int main()
{
    auto server = std::make_shared<ListenServer>("127.0.0.1", 23333);
    server->Start();
    unit::SingleTon<Reactor>::Instance()->RegisterHandler(server, reactor::kReadEvent);
    while(1) {
        unit::SingleTon<Reactor>::Instance()->HandleEvents();
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}