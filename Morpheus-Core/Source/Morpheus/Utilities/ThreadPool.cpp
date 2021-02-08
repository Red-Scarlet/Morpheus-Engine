#include "Morppch.h"
#include "ThreadPool.h"

namespace Morpheus {

	ThreadPool::ThreadPool(const uint32& _Threads)
        : m_NumThreads(_Threads)
	{
        MORP_CORE_TRACE("[THREADPOOL] Num. Threads: " + ToString(m_NumThreads));
	}

    ThreadPool::~ThreadPool()
    {
    }

    void ThreadPool::Start()
    {
        for (size_t i = 0; i < m_NumThreads; ++i)
            m_Threads.emplace_back([this] {
            for (;;) 
            {
                Task task;
                {
                    std::unique_lock<std::mutex> lock(this->m_QueueMutex);
                    this->m_Condition.wait(lock, [this] { return this->m_Stopping || !this->m_Tasks.empty(); });
                    if (this->m_Stopping && this->m_Tasks.empty())
                        return;
                    task = std::move(this->m_Tasks.front());
                    this->m_Tasks.pop();
                }
                task();
            }
        });
	}

    void ThreadPool::Stop()
    {
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Stopping = true;
        }

        m_Condition.notify_all();
        for (auto& thread : m_Threads)
            thread.join();

        m_Threads.clear();
        m_Stopping = true;
    }

    Ref<ThreadPool> ThreadPool::Create(const uint32& _Threads)
    {
        return CreateRef<ThreadPool>(_Threads);
    }
}