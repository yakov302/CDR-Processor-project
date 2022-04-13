#ifndef AGGREGATOR_HPP
#define AGGREGATOR_HPP
 
#include <iostream>
#include <vector>

#include "data_base.hpp"
#include "aggregate_data.hpp"
#include "aggregator.hpp"
#include "thread.hpp"
#include "blocking_queue.hpp"

namespace cdr
{

class Aggregator
{
public:
    Aggregator(DataBase& a_dataBase, mt::BlockingQueue<cdr::Cdr>& a_queue);
    ~Aggregator();

    void runAggregator();
    void stop();

private:
    void aggregate(cdr::Cdr& a_cdr);

    Aggregator(Aggregator const& a_source);
    Aggregator& operator=(Aggregator const& a_source);

private:
    DataBase& m_dataBase;
    mt::BlockingQueue<cdr::Cdr>& m_queue;
    cpp::Thread* m_thread;
    bool m_stop;
};


} //namespace cdr

#endif //AGGREGATOR_HPP