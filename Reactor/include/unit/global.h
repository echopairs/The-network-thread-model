//
// Created by pairs on 4/18/17.
//

#ifndef PROJECT_GLOBAL_H
#define PROJECT_GLOBAL_H

#include <unit/singleton.h>

using namespace reactor;
class reactor::Reactor;


namespace util
{
class Global:public Singleton<Global>
{
public:
    Global(void);
    ~Global();

    std::shared_ptr<Reactor> g_reactor_ptr_;
};
}
#endif //PROJECT_GLOBAL_H
