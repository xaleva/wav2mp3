#include "../include/SystemOutput.h"

#include <iostream>

SystemOutput* SystemOutput::_instance = nullptr;
pthread_mutex_t SystemOutput::_mutex;

SystemOutput& SystemOutput::getInstance ()
{
    pthread_mutex_lock (&_mutex);
    if (_instance == nullptr)
    {
        _instance = new SystemOutput ();
    }
    pthread_mutex_unlock (&_mutex);
    return *_instance;
}

void SystemOutput::output (std::string message)
{
    pthread_mutex_lock (&_mutex);
    std::cout << message << std::endl;
    pthread_mutex_unlock (&_mutex);
}

void SystemOutput::output (const char* message)
{
    pthread_mutex_lock (&_mutex);
    std::cout << message << std::endl;
    pthread_mutex_unlock (&_mutex);
}
