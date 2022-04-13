#ifndef TEMPLATE_QUEUE_HXX
#define TEMPLATE_QUEUE_HXX

#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <exception>

namespace cpp
{

inline QueueExceptions::QueueExceptions(const char *a_error)
: m_error(a_error)
{

}

inline QueueExceptions::~QueueExceptions() throw()
{

}

inline const char *QueueExceptions::what() const throw()
{
    return m_error;
}

template <typename T>
Queue<T>::Queue(size_t a_capacity)
: m_capacity(a_capacity)
, m_size()
, m_in()
, m_out()
, m_array(new T[m_capacity])
{

}

template <typename T>
Queue<T>::Queue(Queue const &source)
: m_capacity(source.m_capacity)
, m_size(source.m_size)
, m_in(source.m_size)
, m_out()
, m_array(new T[m_capacity])
{
    size_t count = 0;
    size_t j = 0;
    size_t i = source.m_out;
    while (count < source.m_size)
    {
        m_array[j] = source.m_array[i];
        i = (i + 1) % source.m_capacity;
        ++count;
        ++j;
    }
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue const &source)
{
    if (this != &source)
    {
        m_capacity = source.m_capacity;
        m_size = source.m_size;
        m_in = source.m_size;
        m_out = 0;

        if (m_capacity != source.m_capacity)
        {
            T *newArr = new T[m_capacity];
            delete[] m_array;
            m_array = newArr;
        }

        size_t count = 0;
        size_t j = 0;
        size_t i = source.m_out;
        while (count < source.m_size)
        {
            m_array[j] = source.m_array[i];
            i = (i + 1) % source.m_capacity;
            ++count;
            ++j;
        }
    }
    return *this;
}

template <typename T>
Queue<T>::~Queue()
{
    delete[] m_array;
}

template <typename T>
void Queue<T>::enqueue(T const &a_value)
{
    if (m_size >= m_capacity)
    {
        throw QueueExceptions("Queue is full!\n");
    }
    m_array[m_in] = a_value;
    m_in = (m_in + 1) % m_capacity;
    m_size++;
}

template <typename T>
void Queue<T>::dequeue(T &a_item)
{
    if (m_size == 0)
    {
        throw QueueExceptions("Queue is empty!\n");
    }
    a_item = m_array[m_out];
    m_out = (m_out + 1) % m_capacity;
    m_size--;
}

template <typename T>
size_t Queue<T>::size() const
{
    return m_size;
}

template <typename T>
bool Queue<T>::full() const
{
    return m_size >= m_capacity;
}

template <typename T>
bool Queue<T>::empty() const
{
    return m_size == 0;
}

template <typename T>
void Queue<T>::clear()
{
    m_in = 0;
    m_out = 0;
    m_size = 0;
}

template <typename T>
std::ostream &Queue<T>::print(std::ostream &a_os) const
{
    size_t i = m_out;
    size_t count = 0;

    while (count < m_size)
    {
        a_os << m_array[i] << ", ";
        i = (i + 1) % m_capacity;
        count++;
    }
    return a_os;
}

template <typename T>
std::ostream &operator<<(std::ostream &a_os, Queue<T> const &a_queue)
{
    return a_queue.print(a_os);
}


} //cpp namespace

#endif // TEMPLATE_QUEUE_HXX