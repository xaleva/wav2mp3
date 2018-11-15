#ifndef FUNCTOR_H
#define FUNCTOR_H

#include <atomic>

class Task
{
    public:
        Task ():
            _isFinished (false)
            {

            }
        virtual ~Task () {}
        virtual void operator() () = 0;

        void finish ()
        {
            _isFinished = true;
        }

        bool isFinished () const
        {
            return _isFinished;
        }
    protected:
        std::atomic<bool> _isFinished;
};


#endif
