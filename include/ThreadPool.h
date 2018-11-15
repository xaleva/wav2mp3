#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <memory>
#include <queue>
#include <vector>
#include <atomic>
#include <stack>
#include <iostream>

#include "pthread.h"
#include "Task.h"


class ThreadPool
{
    protected:
        ThreadPool () = default;
        ~ThreadPool () = default;
    public:
        static ThreadPool& getInstance ();
        void init ();
        void runTask (std::shared_ptr<Task> task);

        void execute ();

        int exec ();
    private:
        static ThreadPool* _instance;
        static pthread_mutex_t _mutex;
        std::atomic<int> _taskCount;
        std::atomic<int> _threadCount;
        pthread_cond_t _conditionVariable;
        std::queue<std::shared_ptr<Task>> _taskQueue;
};


#endif
