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
                if (!_packetQueue.empty ())
                {
                    pthread_mutex_lock (&_mutex);
                    auto packet = _packetQueue.front ();
                    _packetQueue.pop ();
                    pthread_mutex_unlock (&_mutex);

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
        pthread_mutex_t _mutex;
};


#endif
