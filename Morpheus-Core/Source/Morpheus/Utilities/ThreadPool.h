#pragma once

#include "Morpheus/Core/Common.h"

#include <condition_variable>
#include <functional>
#include <vector>
#include <future>
#include <thread>
#include <queue>

namespace Morpheus {

    using Task = Function<void()>;

    class ThreadPool
    {
    public:
        explicit ThreadPool(const uint32& _Threads);
        ~ThreadPool();

        template<class F, class... Args>
        auto Enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;

    public:
        void Start();
        void Stop();

    private:
        Vector<std::thread> m_Threads;
        std::condition_variable m_Condition;
        std::mutex m_QueueMutex;
        std::queue<Task> m_Tasks;
        bool m_Stopping = false;
        uint32 m_NumThreads = 0;
    public:
        static Ref<ThreadPool> Create(const uint32& _Threads);
    };

    template<class F, class... Args>
    inline auto ThreadPool::Enqueue(F&& f, Args&&...args )-> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            if (m_Stopping) throw std::runtime_error("enqueue on stopped ThreadPool");
            m_Tasks.emplace([task]() { (*task)(); });
        }

        m_Condition.notify_one();
        return res;
    }
}