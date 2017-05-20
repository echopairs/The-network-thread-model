//
// Created by pairs on 5/18/17.
//

#include <mreactor/work_reactor.h>
#include <mreactor/conn_task.h>
#include <iostream>
#include <mreactor/reactor.h>

namespace mreactor {

    WorkReactor::WorkReactor() {
        _reactor = std::make_shared<Reactor>();
    }

    WorkReactor::~WorkReactor() {

    }

    bool WorkReactor::Initialize(const std::string &config) {
        if(_is_in_loop) {
            std::cout << "work reactor already running" << std::endl;
            return false;
        }
        _is_inited = true;
        return true;
    }

    bool WorkReactor::Start() {
        if (!_is_inited) {
            std::cout << "reactor manager is not init";
            return false;
        }
        else if (_work_thread.get_id() == std::thread::id()) {
            _work_thread = std::thread([this] {
                _is_in_loop = true;
                try {
                    _reactor->HandleEvents([this]() {
                        _deal_conn_task();
                    });
                } catch(const std::exception &e) {
                    std::cout << "reactor manager CRITICAL ERROR!!! " << e.what() << std::endl;
                }
            });
        }
        return false;
    }

    bool WorkReactor::Stop() {
        _is_in_loop = false;
        if (_work_thread.get_id() != std::thread::id()) {
            std::cout << "stop main reactor " << std::endl;
            // todo
            _work_thread.join();
            _work_thread = std::thread();
            return true;
        } else {
            std::cout << "reactor manager is not running when trying stop";
            return false;
        }
    }

    void WorkReactor::PushTask(std::shared_ptr<ConnTask> event) {
        std::lock_guard<std::mutex> lk(_task_mutex);
        _task_queue.emplace(event);
    }

    void WorkReactor::_deal_conn_task() {
        std::lock_guard<std::mutex> lk(_task_mutex);
        while(_task_queue.size() > 0) {
            auto conn_task = _task_queue.front();
            if (!conn_task) {
                break;
            }
            _task_queue.pop();
            conn_task->get_event_ptr()->set_reactor_ptr(_reactor);
            _reactor->RegisterHandler(conn_task->get_event_ptr(), conn_task->get_evt_type());
        }
    }



}   // namespace mreactor

