//
// Created by pairs on 5/18/17.
//

#ifndef MREACTOR_WORK_REACTOR_H
#define MREACTOR_WORK_REACTOR_H

#include <thread>
#include <mutex>
#include <queue>
#include <atomic>

namespace mreactor {
    class Reactor;
    class IEventHandler;
    class ConnTask;
}

namespace mreactor {

    class WorkReactor
    {
    public:
        WorkReactor();
        ~WorkReactor();
        bool Initialize(const std::string &config);
        bool Start();
        bool Stop();

        void PushTask(std::shared_ptr<ConnTask> event);
    private:

        std::atomic<bool> _is_in_loop;
        std::atomic<bool> _is_inited;

        std::shared_ptr<Reactor> _reactor;
        std::atomic<int> _fds_num;
        std::thread _work_thread;
        void _deal_conn_task();
        std::mutex _task_mutex;
        std::queue<std::shared_ptr<ConnTask> > _task_queue;
    };
}   // namespace mreactor

#endif //MREACTOR_WORK_REACTOR_H
