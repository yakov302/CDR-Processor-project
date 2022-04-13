#ifndef BLOKONG_QUEUE_HPP
#define BLOKONG_QUEUE_HPP

#include <condition_variable>
#include <mutex>

#include "mutex.hpp"
#include "queue.hpp"

namespace mt
{

class BlockingQueueExceptions : public std::exception
{
public:
    BlockingQueueExceptions(const char* a_error);
    ~BlockingQueueExceptions()throw();
    virtual const char* what() const throw();

private:
    const char* m_error;
};

template <typename T>
class BlockingQueue
{
public:
    explicit BlockingQueue(size_t a_capacity = 64);

    void enqueue(T const& a_item);
    void dequeue(T& a_item);
    void clear();

    size_t size() const;
    bool full()const;
    bool empty()const;
    std::ostream& print(std::ostream& a_os = std::cout) const;

private:
    bool nonBlockFull()const;
    bool nonBlockEmpty()const;
    BlockingQueue(BlockingQueue const&  source);            // no impl by design
    BlockingQueue& operator=(BlockingQueue const& source);  // no impl by design

private:
    cpp::Queue<T> m_queue;    
    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cond;
};


} //mt namespace

#include "./inl/blocking_queue.hxx"

#endif // BLOKONG_QUEUE_HPP
