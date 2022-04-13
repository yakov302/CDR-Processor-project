#ifndef BLOKONG_QUEUE_HXX
#define BLOKONG_QUEUE_HXX

#include <exception>
#include <iostream>
#include <string>
#include "mutex.hpp"

namespace mt
{

inline BlockingQueueExceptions::BlockingQueueExceptions(const char *a_error)
: m_error(a_error)
{

}

inline BlockingQueueExceptions::~BlockingQueueExceptions() throw()
{

}

inline const char *BlockingQueueExceptions::what() const throw()
{
    return m_error;
}

typedef std::unique_lock<std::mutex> Lock;

template <typename T>
BlockingQueue<T>::BlockingQueue(size_t a_capacity)
try
: m_queue(a_capacity)
, m_mutex()
, m_cond()
{
    
}
catch(MutexExceptions const& a_exception)
{
    throw BlockingQueueExceptions("BlockingQueue create fail!\n");
}

template <typename T>
void BlockingQueue<T>::enqueue(T const& a_item)
{
    Lock lock(m_mutex);
    m_cond.wait(lock, [this]() {return !nonBlockFull();});
    m_queue.enqueue(a_item);
    m_cond.notify_all();
}

template <typename T>
void BlockingQueue<T>::dequeue(T& a_item)
{
    Lock lock(m_mutex);
    m_cond.wait(lock, [this]() {return !nonBlockEmpty();});
    m_queue.dequeue(a_item);
    m_cond.notify_all();
}

template <typename T>
size_t BlockingQueue<T>::size() const
{
    Lock lock(m_mutex);
    size_t size = m_queue.size();
    return size;
}

template <typename T>
bool BlockingQueue<T>::full() const
{
    Lock lock(m_mutex);
    bool isFull = m_queue.full();
    return isFull;
}

template <typename T>
bool BlockingQueue<T>::nonBlockFull()const
{
    return m_queue.full();
}

template <typename T>
bool BlockingQueue<T>::empty() const
{
    Lock lock(m_mutex);
    bool isEmpty = m_queue.empty();
    return isEmpty;
}

template <typename T>
bool BlockingQueue<T>::nonBlockEmpty()const
{
    return m_queue.empty();
}

template <typename T>
void BlockingQueue<T>::clear()
{
    Lock lock(m_mutex);
    m_queue.clear();
}

template <typename T>
std::ostream& BlockingQueue<T>::print(std::ostream& a_os) const
{
    Lock lock(m_mutex);
    m_queue.print(a_os);
    return a_os;
}


} //mt namespace

#endif // BLOKONG_QUEUE_HXX