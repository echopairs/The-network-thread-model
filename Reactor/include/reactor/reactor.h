//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_REACTOR_H
#define REACTOR_REACTOR_H

namespace reactor
{

class ReactorImpl;
class Reactor
{
public:
    Reactor();
    ~Reactor();
private:
    std::shared_ptr<ReactorImpl> r_impl_;
};
}

#endif //REACTOR_REACTOR_H
