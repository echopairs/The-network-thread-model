//
// Created by pairs on 4/23/17.
//


#include <cstddef>
#include "proactor_impl.h"

namespace proactor {

    ProactorImpl::ProactorImpl(size_t num):thread_pool_(num) {
    }

    int ProactorImpl::RegisterHandler(std::shared_ptr<EventHandle> handler, event_t evt) {
        // TODO
    }

    int ProactorImpl::UnRegisterHandler(handle_t t) {
        // TODO
    }

    void ProactorImpl::HandleEvents() {
        // TODO
    }

    ProactorImpl::~ProactorImpl() {
        // TODO
    }
}

