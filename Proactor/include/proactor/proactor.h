//
// Created by pairs on 4/23/17.
//

#ifndef PROACTOR_PROACTOR_H
#define PROACTOR_PROACTOR_H

#include <proactor/event_handle.h>
#include <memory>

namespace proactor
{
    class ProactorImpl;
    class Proactor
    {
    public:
        Proactor();
        ~Proactor();

        int RegisterHandler(std::shared_ptr<IEventHandler> handler, event_t evt);
        int UnRegisterHandler(handle_t h);

        void HandleEvents();

    private:
        Proactor(const Proactor &);
        Proactor &operator=(const Proactor &);
        std::shared_ptr<ProactorImpl> proactor_impl_ptr_;
    };
}   // namespace proactor
#endif //PROACTOR_PROACTOR_H
