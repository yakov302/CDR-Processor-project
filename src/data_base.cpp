#include "data_base.hpp"

namespace cdr
{

Lock::Lock(mt::Mutex& a_mutex)
try
: m_mutex(a_mutex)
{
    m_mutex.lock();
}
catch(mt::MutexExceptions const& a_exception)
{
    std::cout << "Unable to lock!\n";
}

Lock::~Lock()
try
{
    m_mutex.unlock(); 
}
catch(mt::MutexExceptions const& a_exception)
{
    std::cout << "Unable to unlock!\n";
}

DataBaseExceptions::DataBaseExceptions(const char *a_error)
: m_error(a_error)
{

}

DataBaseExceptions::~DataBaseExceptions() throw()
{

}

const char* DataBaseExceptions::what() const throw()
{
    return m_error;
}

DataBase::DataBase()
try
: m_mutex()
, m_opMutex()
, m_data()
, m_opData()
{

}
catch(mt::MutexExceptions const& a_exception)
{
    throw DataBaseExceptions("BlockingQueue create fail!\n");
}

void DataBase::loadAggData(AggData a_aggData)
{
    Lock lock(m_mutex);
    if(!isExsistNoLock(a_aggData.m_msisdn))
    {
        insertNewSubscriber(a_aggData);
    }
    else
    {
        updateExsistSubscriber(a_aggData);
    }
}

void DataBase::loadOpAggData(OpAggData a_opAggData)
{
    Lock lock(m_opMutex);
    if(!isOpExsistNoLock(a_opAggData.m_mccMnc))
    {
        insertNewOperator(a_opAggData);
    }
    else
    {
        updateExsistOperator(a_opAggData);
    }
}

bool DataBase::isExsist(std::string a_msisdn) const
{
    Lock lock(m_mutex);
    auto it = m_data.find(a_msisdn);
    if(it != m_data.end())
    {
        return true;
    }
    return false;
}

bool DataBase::isOpExsistNoLock(std::string a_mnc) const
{
    auto it = m_opData.find(a_mnc);
    if(it != m_opData.end())
    {
        return true;
    }
    return false;
}

bool DataBase::isExsistNoLock(std::string a_msisdn) const
{
    auto it = m_data.find(a_msisdn);
    if(it != m_data.end())
    {
        return true;
    }
    return false; 
}

void DataBase::insertNewSubscriber(AggData a_aggData)
{
    m_data.insert({a_aggData.m_msisdn, a_aggData});
}

 void DataBase::insertNewOperator(OpAggData a_opAggData)
 {
    m_opData.insert({a_opAggData.m_mccMnc, a_opAggData});
 }

void loadVoiceLinks(std::unordered_map<std::string, size_t>& a_voiseLinks, std::pair<std::string, size_t> a_sp)
{
    auto it = a_voiseLinks.find(a_sp.first);
    if (it == a_voiseLinks.end())
    {
        a_voiseLinks.insert({a_sp.first, a_sp.second});
    }
    else
    {
        a_voiseLinks[a_sp.first] += a_sp.second;
    }
}

void loadSmsLinks(std::unordered_map<std::string, size_t>& a_smsLinks, std::string a_sp)
{
    auto it = a_smsLinks.find(a_sp);
    if (it == a_smsLinks.end())
    {
        a_smsLinks.insert({a_sp, 1});
    }
    else
    {
        a_smsLinks[a_sp] += 1;
    }
}

 void DataBase::updateExsistSubscriber(AggData a_aggData)
 {
    m_data[a_aggData.m_msisdn].m_dataIn += a_aggData.m_dataIn;
    m_data[a_aggData.m_msisdn].m_dataOut += a_aggData.m_dataOut;
    m_data[a_aggData.m_msisdn].m_smsIn += a_aggData.m_smsIn;
    m_data[a_aggData.m_msisdn].m_smsOut += a_aggData.m_smsOut;
    m_data[a_aggData.m_msisdn].m_voiceIn += a_aggData.m_voiceIn;
    m_data[a_aggData.m_msisdn].m_voiceOut += a_aggData.m_voiceOut;
    for(auto i : a_aggData.m_voiceLinks)
    {
        std::pair<std::string, size_t> sp;
        sp.first = i.first;
        sp.second = i.second;
        loadVoiceLinks(m_data[a_aggData.m_msisdn].m_voiceLinks, sp);
    }
    for(auto i : a_aggData.m_smsLinks)
    {
        loadSmsLinks(m_data[a_aggData.m_msisdn].m_smsLinks, i.first);
    }
}

void DataBase::updateExsistOperator(OpAggData a_opAggData)
{
    m_opData[a_opAggData.m_mccMnc].m_smsIn += a_opAggData.m_smsIn;
    m_opData[a_opAggData.m_mccMnc].m_smsOut += a_opAggData.m_smsOut;
    m_opData[a_opAggData.m_mccMnc].m_voiceIn += a_opAggData.m_voiceIn;
    m_opData[a_opAggData.m_mccMnc].m_voiceOut += a_opAggData.m_voiceOut;
}

AggData DataBase::giveAggDataByMsisdn(std::string a_msisdn)
{
    Lock lock(m_mutex);
    return m_data[a_msisdn];
}

OpAggData DataBase::giveAggDataByMnc(std::string a_mccMnc)
{
    Lock lock(m_opMutex);
    return m_opData[a_mccMnc];
}

std::ostream& DataBase::printDataBaseByMsisdn(std::ostream& a_os) const
{
    Lock lock(m_mutex);
    for(auto it : m_data)
    {
        a_os << "Subscriber: " << (it).second.m_msisdn << "\n";
        a_os << "Voice out - " << (it).second.m_voiceOut << "\n";
        a_os << "Voice in  - " << (it).second.m_voiceIn << "\n";
        a_os << "Sms out   - " << (it).second.m_smsOut << "\n";
        a_os << "Sms in    - " << (it).second.m_smsIn << "\n";
        a_os << "Data out  - " << (it).second.m_dataOut << "\n";
        a_os << "Data in   - " << (it).second.m_dataIn << "\n";
        for(auto i : it.second.m_voiceLinks)
        {
            a_os << "Second party " << i.first << " Voice - " << i.second << "\n";
        }
        for(auto i : it.second.m_smsLinks)
        {
            a_os << "Second party " << i.first << " Sms   - " << i.second << "\n";
        }
        a_os << "\n";
    }
    return a_os;
}

std::ostream& DataBase::printDataBaseByNmc(std::ostream& a_os) const
{
    Lock lock(m_opMutex);
    for(auto it : m_opData)
    {
        a_os << "Operator  : " << (it).second.m_mccMnc << "\n";
        a_os << "Voice out - " << (it).second.m_voiceOut << "\n";
        a_os << "Voice in  - " << (it).second.m_voiceIn << "\n";
        a_os << "Sms out   - " << (it).second.m_smsOut << "\n";
        a_os << "Sms in    - " << (it).second.m_smsIn << "\n";
        a_os << "\n";
    }
    return a_os;
}


} //cdr namespace