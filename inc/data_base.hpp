#ifndef DATA_BASE_HPP
#define DATA_BASE_HPP

#include<iostream>
#include <utility>
#include <map>
#include <unordered_map>

#include "mutex.hpp"
#include "cdr_struct.hpp"
#include "aggregate_data.hpp"

namespace cdr
{

class Lock
{
public:
    Lock(mt::Mutex& a_mutex);
    ~Lock();

private:
    Lock(Lock const&);
    Lock& operator=(Lock const&);

private:
    mt::Mutex& m_mutex;
};

class DataBaseExceptions : public std::exception
{
public:
    DataBaseExceptions(const char* a_error);
    ~DataBaseExceptions()throw();
    virtual const char* what() const throw();

private:
    const char* m_error;
};

class DataBase
{
public:
    DataBase();

    void loadAggData(AggData a_aggData);
    void loadOpAggData(OpAggData a_opAggData);

    AggData giveAggDataByMsisdn(std::string a_msisdn);
    OpAggData giveAggDataByMnc(std::string a_mnc);

    bool isExsist(std::string a_msisdn) const;
    std::ostream& printDataBaseByMsisdn(std::ostream& a_os = std::cout) const;
    std::ostream& printDataBaseByNmc(std::ostream& a_os = std::cout) const;
    
private:
    bool isExsistNoLock(std::string a_msisdn) const;
    bool isOpExsistNoLock(std::string a_mnc) const;

    void insertNewSubscriber(AggData a_aggData);
    void insertNewOperator(OpAggData a_opAggData);

    void updateExsistSubscriber(AggData a_aggData);
    void updateExsistOperator(OpAggData a_opAggData);

private:
    mutable mt::Mutex m_mutex;
    mutable mt::Mutex m_opMutex;
    std::unordered_map <std::string, AggData> m_data;
    std::unordered_map <std::string, OpAggData> m_opData;
};


} //cdr namespace

#endif // DATA_BASE_HPP