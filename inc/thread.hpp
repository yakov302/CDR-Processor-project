#ifndef THREAD_HPP
#define THREAD_HPP

#include <exception>
#include <cassert>
#include <string>

namespace cpp
{

class ThreadExceptions : public std::exception
{
public:
    ThreadExceptions(std::string const& a_functionName, int const& a_errorNumber);
    ~ThreadExceptions()throw();
    virtual const char* what() const throw();
    std::string getFunctionName()const throw();
    int getErrorNumber()const throw();

private:
    std::string m_functionName;
    int m_errorNumber;
};

class Thread{
public:
    Thread(void*(*a_fx)(void*), void* a_argument = 0);
    Thread(Thread const& a_src);
    ~Thread();
    void* join();
    void detach();

private:
    pthread_t m_tid;
    mutable bool m_wasJoined;
};

 
} //cpp namespace

#include "./inl/thread.hxx"

#endif  //THREAD_HPP