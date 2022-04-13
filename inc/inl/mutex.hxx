#ifndef MUTEX_HXX
#define MUTEX_HXX

#include <exception>
#include <cassert>
#include <iostream>

namespace mt
{

inline MutexExceptions::MutexExceptions( std::string const& a_functionName, int const& a_errorNumber)
: m_functionName(a_functionName)
, m_errorNumber(a_errorNumber)
{

}

inline MutexExceptions::~MutexExceptions() throw()
{

}

inline const char* MutexExceptions::what() const throw()
{
    return "Mutex fail!\n";
}

inline std::string MutexExceptions::getFunctionName() const throw()
{
    return m_functionName;
}

inline int MutexExceptions:: getErrorNumber()const throw()
{
    return m_errorNumber;
}

inline Mutex::Mutex()
: m_mutex()
{
    int r = pthread_mutex_init(&m_mutex, 0);
    if (r != 0)
    {
        throw MutexExceptions("pthread_mutex_init", r);
    }
}

inline Mutex::~Mutex()
{ 
    int r = pthread_mutex_destroy(&m_mutex);
    if (r != 0)
    {
        std::cout << "pthread_mutex_destroy fail! error number: " << r << "\n";
    }
}

inline void Mutex::lock()
{
    int r =  pthread_mutex_lock(&m_mutex);
    if (r != 0)
    {
       throw MutexExceptions("pthread_mutex_lock", r);
    }
}

inline void Mutex::unlock()
{
    int r = pthread_mutex_unlock(&m_mutex); 
    if (r != 0)
    {
        throw MutexExceptions("pthread_mutex_unlock", r);
    }
}


} //mt namespace

#endif  //MUTEX_HXX