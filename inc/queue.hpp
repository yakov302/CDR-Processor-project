#ifndef TEMPLATE_QUEUE_H
#define TEMPLATE_QUEUE_H

#include <cstddef>
#include <exception>

namespace cpp{

class QueueExceptions : public std::exception
{
public:
    QueueExceptions(const char* a_error);
    ~QueueExceptions()throw();
    virtual const char* what() const throw();

private:
    const char* m_error;
};

template <typename T>
class Queue
{
public:
    Queue(size_t a_capacity = 64);
    Queue(Queue const&  source);
    Queue& operator=(Queue const& source);
    ~Queue();

    void enqueue(T const& a_value);
    void dequeue(T& a_item);
    void clear();

    size_t size() const;
    bool full()const;
    bool empty()const;
    std::ostream& print(std::ostream& a_os = std::cout) const;

private:
    void axioms() const;

private:
    size_t m_capacity;
    size_t m_size;
    size_t m_in;
    size_t m_out;
    T* m_array;
};

template <typename T>
std::ostream& operator<<(std::ostream& a_os, Queue<T> const& a_queue);


} //cpp namespace

#include "./inl/queue.hxx"

#endif //TEMPLATE_QUEUE_H122