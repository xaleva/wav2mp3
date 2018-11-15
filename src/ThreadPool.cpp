#include "../include/ThreadPool.h"
#include "../include/SystemOutput.h"

#include <iostream>

ThreadPool* ThreadPool::_instance = nullptr;
pthread_mutex_t ThreadPool::_mutex;

#define THREAD_COUNT 50
#define THREAD_THREASHOLD 5

void* processor_executor(void* args)
{
    ThreadPool *processor = static_cast<ThreadPool*>(args);
    processor->execute ();
    return NULL;
}

ThreadPool& ThreadPool::getInstance ()
{
    pthread_mutex_lock (&_mutex);
    if (_instance == nullptr)
    {
        _instance = new ThreadPool ();
    }
    pthread_mutex_unlock (&_mutex);
    return *_instance;
}

void ThreadPool::init ()
{
    for (int i = 0;i < THREAD_COUNT; ++i)
    {
        pthread_t id;
        int err = pthread_create (&id, NULL, processor_executor, static_cast<void*> (this));
        _threadCount++;
    }
}

void ThreadPool::runTask (std::shared_ptr<Task> task)
{
    pthread_mutex_lock (&_mutex);
    _taskCount++;
    _taskQueue.push (task);
    if (_taskCount > _threadCount - THREAD_THREASHOLD)
    {
        _threadCount++;
        pthread_t id;
        int err = pthread_create (&id, NULL, processor_executor, static_cast<void*> (this));
    }
    pthread_mutex_unlock (&_mutex);
    pthread_cond_signal (&_conditionVariable);
}

void ThreadPool::execute ()
{
    while (true)
    {
        pthread_mutex_lock (&_mutex);
        while (_taskQueue.empty ())
            pthread_cond_wait (&_conditionVariable, &_mutex);
        auto task = _taskQueue.front ();
        _taskQueue.pop ();
        pthread_mutex_unlock (&_mutex);

        (*task.get ()) ();
        _taskCount--;
    }
}

int ThreadPool::exec ()
{
    while (_taskCount > 0)
    {
    }
    return 0;
}
