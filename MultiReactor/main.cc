#include <iostream>
#include <mreactor/reactor_manager.h>
using namespace std;


using namespace mreactor;

int main() {

    auto reactor_manager = std::make_shared<ReactorManager>("127.0.0.1", 23333);;
    reactor_manager->Start();
    while(1) {

    }
}