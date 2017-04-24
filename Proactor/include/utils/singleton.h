//
// Created by pairs on 4/17/17.
//

#ifndef PROACTOR_SINGLETON_H
#define PROACTOR_SINGLETON_H

#include <memory>
#include <mutex>

namespace utils
{
template <typename T>
class SingleTon
{
public:
    template <typename... Args>
    static std::shared_ptr<T> Instance(Args&&... args)
    {
        if (pInstance_ == nullptr)
        {
            std::lock_guard<std::mutex>lock(mtx_);
            if (pInstance_ == nullptr)
            {
                pInstance_ = std::make_shared<T>(std::forward<Args>(args)...);
            }
        }
        return pInstance_;
    }
private:
    static std::mutex mtx_;
    static std::shared_ptr<T> pInstance_;
    virtual ~SingleTon();
    SingleTon();
};

template <typename T> std::shared_ptr<T> SingleTon<T>::pInstance_ = nullptr;
template <typename T> std::mutex SingleTon<T>::mtx_;
}


#endif //PROACTOR_SINGLETON_H
