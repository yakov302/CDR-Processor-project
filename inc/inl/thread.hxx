#ifndef THREAD_HXX
#define THREAD_HXX

#include <exception>

namespace cpp
{

inline ThreadExceptions::ThreadExceptions( std::string const& a_functionName, int const& a_errorNumber)
: m_functionName(a_functionName)
, m_errorNumber(a_errorNumber)
{

}

inline ThreadExceptions::~ThreadExceptions() throw()
{

}

inline const char* ThreadExceptions::what() const throw()
{
    return "Thread fail!\n";
}

inline std::string ThreadExceptions::getFunctionName() const throw()
{
    return m_functionName;
}

inline int ThreadExceptions:: getErrorNumber()const throw()
{
    return m_errorNumber;
}


inline Thread::Thread(void* (*a_fx)(void*), void* a_argument)
: m_tid(0)
, m_wasJoined(false)
{
    int r = ::pthread_create(&m_tid, 0, a_fx, a_argument);
    if (r != 0)
    {
        throw ThreadExceptions("pthread_create", r);
    }
}

inline Thread::Thread(Thread const& a_src)
: m_tid(a_src.m_tid)
, m_wasJoined(a_src.m_wasJoined)
{
    Thread &t = const_cast<Thread &>(a_src);
    t.m_wasJoined = true;
    t.m_tid = 0;
}

inline Thread::~Thread()
{
    assert(m_wasJoined);
}

inline void* Thread::join()
{
    void* result = 0;
    int r = ::pthread_join(m_tid, &result);
    if (r != 0)
    {
        throw ThreadExceptions("pthread_join", r);
    }
    m_wasJoined = true;
    return result;
}

inline void Thread::detach()
{
    int r = ::pthread_detach(m_tid);
    if (r != 0)
    {
        throw ThreadExceptions("pthread_detach", r);
    }
}


} //cpp namespace

#endif  // THREAD_HXX