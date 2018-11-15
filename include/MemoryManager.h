#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <unordered_map>
#include <stack>
#include <memory>
#include <string.h>

#include "pthread.h"

#define CHUNK_ELEMENT_COUNT 8192

template<class T>
class MemoryChunk
{
    public:
        MemoryChunk (long size):
            _size (size)
        {
            _data = new T [size];
            _usedSize = size;
            memset (_data, 0, size);
        }

        ~MemoryChunk ()
        {
            delete [] _data;
        }

        int getType () const
        {
            return _type;
        }

        long getSize () const
        {
            return _size;
        }

        long setUsedSize (long size)
        {
            _usedSize = size;
        }

        long getUsedSize () const
        {
            return _usedSize;
        }

        T* data () const
        {
            return _data;
        }
    private:
        long _size;
        long _usedSize;
        int _type;
        T* _data;
};

template<class T>
class MemoryManager
{
    public:
        static MemoryManager<T>& getInstance ()
        {
            pthread_mutex_lock (&_mutex);
            if (_instance == nullptr)
            {
                _instance = new MemoryManager<T> ();
            }
            pthread_mutex_unlock (&_mutex);
            return *_instance;
        }

        std::shared_ptr<MemoryChunk<T>> allocateChunk (int size)
        {
            std::shared_ptr<MemoryChunk<T>> chunk;
            pthread_mutex_lock (&_mutex);

            auto deletor = [this] (MemoryChunk<T>* releasedChunk) {
                this->releaseChunk (releasedChunk);
            };

            auto itr = _memoryMap.find (size);
            if (itr != _memoryMap.end ())
            {
                auto& memoryStack = itr->second;
                chunk = std::shared_ptr<MemoryChunk<T>> (memoryStack.top (), deletor);
                memoryStack.pop ();
                if (memoryStack.empty ())
                {
                    _memoryMap.erase (size);
                }
                pthread_mutex_unlock (&_mutex);
            }
            else
            {
                pthread_mutex_unlock (&_mutex);
                chunk = std::shared_ptr<MemoryChunk<T>> (new MemoryChunk<T> (size), deletor);
            }
            return chunk;
        }
    private:
        void releaseChunk (MemoryChunk<T>* chunk)
        {
            pthread_mutex_lock (&_mutex);
            int size = chunk->getSize ();
            auto itr = _memoryMap.find (size);
            if (itr != _memoryMap.end ())
            {
                auto& memoryStack = itr->second;
                memoryStack.push (chunk);
            }
            else
            {
                auto& memoryStack = _memoryMap [size];
                memoryStack.push (chunk);
            }
            pthread_mutex_unlock (&_mutex);
        }
    private:
        MemoryManager () = default;
        ~MemoryManager () = default;
    private:
        static MemoryManager<T>* _instance;
        std::unordered_map<int, std::stack<MemoryChunk<T>* > > _memoryMap;
        static pthread_mutex_t _mutex;
};

template<class T>
MemoryManager<T>* MemoryManager<T>::_instance = nullptr;

template<class T>
pthread_mutex_t MemoryManager<T>::_mutex;

#endif
