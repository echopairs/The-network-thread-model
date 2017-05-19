//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_REACTOR_MANAGER_H
#define MREACTOR_REACTOR_MANAGER_H

#include <string>
#include <memory>
#include <queue>
#include <list>
#include <thread>
#include <atomic>
#include "conn_task.h"
#include "work_reactor.h"

namespace mreactor {
    class WorkReactor;
    class IEventHandler;
    class Reactor;
    class ConnTask;
}

namespace mreactor {

    class ReactorManager
    {
    public:
        ReactorManager(const std::string &ip, int port, int thread_num = 4);
        ~ReactorManager();

        bool Initialize(const std::string &config);
        bool Start();
        bool Stop();

        std::shared_ptr<WorkReactor> VoteWorkReactor();
    private:
        std::shared_ptr<Reactor> _main_reactor;
        std::thread _main_thread;

        void _StartWorkReactor();
        void Notify(std::shared_ptr<ConnTask>);

        std::shared_ptr<WorkReactor> Pop();
        void Push(std::shared_ptr<WorkReactor> work_reactor);
        std::list<std::shared_ptr<WorkReactor> > _work_reactors;

        std::atomic<bool> _is_in_loop;
        std::atomic<bool> _is_inited;
    };
}   // namespace mreactor
#endif //MREACTOR_REACTOR_MANAGER_H
