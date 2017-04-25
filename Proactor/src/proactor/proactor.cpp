//
// Created by pairs on 4/23/17.
//

#include <proactor/proactor.h>
#include "proactor_impl/proactor_impl.h"

namespace proactor
{
    Proactor::Proactor() {
        proactor_impl_ptr_ = std::make_shared<ProactorImpl>(4);
    }

    Proactor::~Proactor() {

    }

    int Proactor::RegisterHandler(std::shared_ptr<EventHandle> handler, event_t evt) {
        proactor_impl_ptr_->RegisterHandler(handler, evt);
    }

    int Proactor::UnRegisterHandler(handle_t h) {
        proactor_impl_ptr_->UnRegisterHandler(h);
    }
}