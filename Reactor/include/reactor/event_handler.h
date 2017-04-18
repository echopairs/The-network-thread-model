//
// Created by pairs on 4/18/17.
//

#ifndef REACTOR_EVENT_HANDLER_H
#define REACTOR_EVENT_HANDLER_H

namespace reactor
{
class IEventHandler
{
public:
    virtual void HandleRead() = 0;
    virtual void HandleWrite() = 0;
    virtual void HandleError() = 0;
protected:
    IEventHandler(){}
    virtual ~IEventHandler(){}
};
}   // namespace reactor
#endif //REACTOR_EVENT_HANDLER_H
