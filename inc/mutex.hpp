#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <exception>
#include <pthread.h>
#include <string>

namespace mt
{

class MutexExceptions : public std::exception
{
public:
    MutexExceptions( std::string const& a_functionName, int const& a_errorNumber);
    ~MutexExceptions()throw();
    virtual const char* what() const throw();
    std::string getFunctionName()const throw();
    int getErrorNumber()const throw();

private:
    std::string m_functionName;
    int m_errorNumber;
};

class Mutex{
public:
    Mutex();
    ~Mutex();
    void lock();
    void unlock();
    
private:
    Mutex(Mutex const& a_mutex);             // no impl by design
    Mutex& operator=(Mutex const& a_mutex);  // no impl by design

private:
    pthread_mutex_t m_mutex;
};


} //mt namespace

#include "./inl/mutex.hxx"

#endif  //MUTEX_HPP