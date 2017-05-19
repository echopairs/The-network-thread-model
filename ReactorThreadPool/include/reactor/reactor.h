//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_REACTOR_H
#define REACTOR_REACTOR_H

#include <reactor/event_handler.h>
namespace reactor
{

class ReactorImpl;
class Reactor
{
public:
    Reactor();
    ~Reactor();
    int RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt);
    int UnRegisterHandler(handle_t h);

    void HandleEvents();
    int RegisterTimerTask();

private:
    Reactor(const Reactor&);
    Reactor &operator=(const Reactor &);
    std::shared_ptr<ReactorImpl> reactor_impl_ptr_;
};
}

#endif //REACTOR_REACTOR_H
