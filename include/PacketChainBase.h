#ifndef PACKET_CHAIN_BASE_H
#define PACKET_CHAIN_BASE_H

#include <queue>

#include "pthread.h"

#include "Packet.h"
#include "Task.h"
#include "ThreadPool.h"

class PacketChainBase:
    public Task
{
    public:
        void setNext (std::shared_ptr<PacketChainBase> next)
        {
            _next = next;
            ThreadPool::getInstance ().runTask (_next);
        }


        virtual void operator() () override
        {
            while (!_isFinished || !_packetQueue.empty ())
            {
                std::shared_ptr<Packet> packet;

                pthread_mutex_lock (&_mutex);
                while (!_isFinished || _packetQueue.empty ())
                    pthread_cond_wait (&_conditionVariable, &_mutex);

                if (!_packetQueue.empty ())
                {
                    packet = _packetQueue.front ();
                    _packetQueue.pop ();
                }
                pthread_mutex_unlock (&_mutex);

                if (packet)
                {
                    execute (packet);
                    forward (packet);
                }
            }
        }

        virtual void execute (std::shared_ptr<Packet> packet) = 0;

        void forward (std::shared_ptr<Packet> packet)
        {
            if (_next)
            {
                _next->append (packet);
            }
        }

        void append (std::shared_ptr<Packet> packet)
        {
            pthread_mutex_lock (&_mutex);
            _packetQueue.push (packet);
            pthread_mutex_unlock (&_mutex);
            pthread_cond_signal (&_conditionVariable);
        }

        virtual ~PacketChainBase ()
        {
            if (_next)
            {
                _next->finish ();
            }
        }
    protected:
        std::shared_ptr<PacketChainBase> _next;
        std::queue<std::shared_ptr<Packet>> _packetQueue;
};


#endif
