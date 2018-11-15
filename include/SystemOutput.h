#ifndef SYSTEM_OUTPUT_H
#define SYSTEM_OUTPUT_H

#include "pthread.h"
#include <string>

class SystemOutput
{
    public:
        static SystemOutput& getInstance ();

        void output (std::string message);
        void output (const char* message);
    private:
        SystemOutput () = default;
        ~SystemOutput () = default;
    private:
        static SystemOutput* _instance;
        static pthread_mutex_t _mutex;
};

#endif
