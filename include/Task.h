#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <atomic>

class Task
{
    public:
        Task ():
            _isFinished (false)
            {
                pthread_mutex_init (&_mutex, NULL);
                pthread_cond_init (&_conditionVariable, NULL);
            }
        virtual ~Task () {}
        virtual void operator() () = 0;

        void finish ()
        {
            _isFinished = true;
            pthread_cond_signal (&_conditionVariable);
        }

        bool isFinished () const
        {
            return _isFinished;
        }
    protected:
        std::atomic<bool> _isFinished;
        pthread_mutex_t _mutex;
        pthread_cond_t _conditionVariable;
};


#endif
