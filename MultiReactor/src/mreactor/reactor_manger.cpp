//
// Created by pairs on 5/18/17.
//

#include <mreactor/reactor_manager.h>
#include <mreactor/listen_handler.h>
#include <mreactor/reactor.h>
#include <mreactor/work_reactor.h>

namespace mreactor {

    ReactorManager::ReactorManager(const std::string &ip, int port, int thread_num):_is_in_loop(false),_is_inited(false) {

        // 1. create listen handle && set listen handle && begin listen
        auto listen_handle = std::make_shared<ListenHandler>(ip, port);
        listen_handle->set_this_shared_ptr_(listen_handle);
        listen_handle->Start();
        listen_handle->set_register_cb([this](const std::shared_ptr<ConnTask> conn) {
            Notify(conn);
        });

        // 2. create main reactor && register listen_handle to main reactor
        _main_reactor = std::make_shared<Reactor>();
        listen_handle->set_reactor_ptr(_main_reactor);
        _main_reactor->RegisterHandler(listen_handle, mreactor::kReadEvent);

        // 3. create work reactor
        for(int i = 0; i < thread_num; i++) {
            _work_reactors.push_back(std::make_shared<mreactor::WorkReactor>());
        }



        // some time may be need init something with config
        Initialize("");
    }

    ReactorManager::~ReactorManager() {
        Stop();
    }

    void ReactorManager::Notify(std::shared_ptr<ConnTask> conn_task) {
        auto work_reactor = VoteWorkReactor();
        if (work_reactor == nullptr) {
            std::cout << "vote work reactor failed, please create work reactor" << std::endl;
            return;
        }
        work_reactor->PushTask(conn_task);
    }

    bool ReactorManager::Start() {
        if (!_is_inited) {
            std::cout << "reactor manager is not init";
            return false;
        }
        else if (_main_thread.get_id() == std::thread::id()) {

            // 1. run work reactor first
            _StartWorkReactor();
            _main_thread = std::thread([this] {
               _is_in_loop = true;
                try {
                    _main_reactor->HandleEvents();
                } catch(const std::exception &e) {
                    std::cout << "reactor manager CRITICAL ERROR!!! " << e.what() << std::endl;
                }
            });
        }
    }

    bool ReactorManager::Stop() {
        _is_in_loop = false;
        if (_main_thread.get_id() != std::thread::id()) {
            std::cout << "stop main reactor " << std::endl;
            // todo
            _main_thread.join();
            _main_thread = std::thread();
            return true;
        } else {
            std::cout << "reactor manager is not running when trying stop";
            return false;
        }
    }

    bool ReactorManager::Initialize(const std::string &config) {
        if(_is_in_loop) {
            std::cout << "reactor manager already running" << std::endl;
            return false;
        }

        for(auto &item : _work_reactors) {
            item->Initialize(config);
        }
        _is_inited = true;
        return true;
    }

    std::shared_ptr<WorkReactor> ReactorManager::VoteWorkReactor() {
        return Pop();
    }

    std::shared_ptr<WorkReactor> ReactorManager::Pop() {

        if (_work_reactors.size() > 0) {
            auto r = _work_reactors.front();
            _work_reactors.pop_front();
            _work_reactors.push_back(r);
            return r;
        } else {
            return nullptr;
        }
    }

    void ReactorManager::Push(std::shared_ptr<WorkReactor> work_reactor) {
        _work_reactors.push_back(work_reactor);
    }

    void ReactorManager::_StartWorkReactor() {
        for(auto &item : _work_reactors) {
            item->Start();
        }
    }


}   // namespace mreactor