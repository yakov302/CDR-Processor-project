#include "aggregator.hpp"

namespace cdr
{

namespace impel
{

std::string giveOperatorMccMnc(std::string a_imsi)
{
    return a_imsi.substr(0, 5);
}

OpAggData aggregateOperatorData(cdr::Cdr a_cdr)
{
    OpAggData data;
    data.m_mccMnc = giveOperatorMccMnc(a_cdr.m_imsi);
    switch (a_cdr.m_usageType)
    {
        case MOC:
            data.m_voiceOut = a_cdr.m_duration;
            break;

        case MTC:
            data.m_voiceIn = a_cdr.m_duration;
            break;

        case SMS_MO:
            data.m_smsOut = 1;
            break;

        case SMS_MT:
            data.m_smsIn = 1;
            break;

        default:
            break;
    }
    
    return data;
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

void loadSmsLinks(std::unordered_map<std::string, size_t>&  a_smsLinks, std::string a_sp)
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

AggData aggregateSubscriberData(cdr::Cdr a_cdr)
{
    AggData data;
    std::pair<std::string, size_t> sp;
    data.m_msisdn = a_cdr.m_msisdn;
    switch (a_cdr.m_usageType)
    {
        case MOC:
            data.m_voiceOut = a_cdr.m_duration;
            sp.first = a_cdr.m_secondPartyMsisdn;
            sp.second = a_cdr.m_duration;
            loadVoiceLinks(data.m_voiceLinks, sp);
            break;

        case MTC:
            data.m_voiceIn = a_cdr.m_duration;
            sp.first = a_cdr.m_secondPartyMsisdn;
            sp.second = a_cdr.m_duration;
            loadVoiceLinks(data.m_voiceLinks, sp);
            break;

        case SMS_MO:
            data.m_smsOut = 1;
            loadSmsLinks(data.m_smsLinks, a_cdr.m_secondPartyMsisdn);
            break;

        case SMS_MT:
            data.m_smsIn = 1;
            loadSmsLinks(data.m_smsLinks, a_cdr.m_secondPartyMsisdn);
            break;

        case D:
            data.m_dataIn = a_cdr.m_bytesReceived;
            data.m_dataOut = a_cdr.m_bytesTransmitted;
            break;

        default:
            break;
    }

    return data;
}

void* threadFunction(void* a_arg)
{
    Aggregator* ag = static_cast<Aggregator*>(a_arg);
    ag->runAggregator();
    return 0;
}


} //namespace impel

Aggregator::Aggregator(DataBase& a_dataBase, mt::BlockingQueue<cdr::Cdr>& a_queue)
: m_dataBase(a_dataBase)
, m_queue(a_queue)
, m_stop(false)
{
    m_thread = new cpp::Thread(impel::threadFunction, this);
}

Aggregator::~Aggregator()
{
    m_thread->join();
    delete m_thread;
}

void Aggregator::aggregate(cdr::Cdr& a_cdr)
{
    OpAggData op_data = impel::aggregateOperatorData(a_cdr);
    m_dataBase.loadOpAggData(op_data);

    AggData data = impel::aggregateSubscriberData(a_cdr);
    m_dataBase.loadAggData(data);
}

void Aggregator::runAggregator()
{
    while (!m_stop)
    {
        cdr::Cdr cdr;
        m_queue.dequeue(cdr);
        if(!cdr.poisoned)
        {
            aggregate(cdr);
        }
    }
}

void Aggregator::stop()
{
    m_stop = true;
}


}//namespace cdr