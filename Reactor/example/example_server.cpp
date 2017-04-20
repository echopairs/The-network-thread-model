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
    while(1) {

    }
}