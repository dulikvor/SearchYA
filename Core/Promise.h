#pragma once

#include <memory>

namespace core{

    class AsyncTask;

    class Promise{
    public:
        std::shared_ptr<AsyncTask> GetTask(const std::function<void(void)>& requestedPoint);
        bool GetValueSet() const{
            return m_valueSet;
        }
        Promise(const Promise&) = delete;
        Promise& operator=(const Promise&) = delete;
    protected:
        Promise():m_valueSet(false){}
        void NotifyOnPromiseSet();

    protected:
        bool m_valueSet;

    private:
        std::weak_ptr<AsyncTask> m_task;
    };

    template<typename T>
    class ConcretePromise : public Promise
    {
    public:
        ConcretePromise(){}
        virtual ~ConcretePromise(){}

        operator T() const{
            return m_value;
        }
        void SetValue(const T& value){
            m_value = value;
            m_valueSet = true;
            NotifyOnPromiseSet();
        }

    private:
        T m_value;
    };
}
