#ifndef CDR_STRUCT_HPP
#define CDR_STRUCT_HPP

#include<iostream>
#include <string>
 
namespace cdr
{

enum UsageType {MOC, MTC, SMS_MO, SMS_MT, D, U, B, X};

class Cdr
{
public:
    Cdr()
    : m_sequenceNumber(0)
    , m_imsi()
    , m_imei()
    , m_usageType()
    , m_msisdn()
    , m_callDate()
    , m_callTime()
    , m_duration(0)
    , m_bytesReceived(0)
    , m_bytesTransmitted(0)
    , m_secondPartyImsi()
    , m_secondPartyMsisdn()
    , poisoned(false){}

    size_t m_sequenceNumber;
    std::string m_imsi;
    std::string m_imei;
    UsageType m_usageType;
    std::string m_msisdn;
    std::string m_callDate;
    std::string m_callTime;
    size_t m_duration;
    size_t m_bytesReceived;
    size_t m_bytesTransmitted;
    std::string m_secondPartyImsi;
    std::string m_secondPartyMsisdn;
    bool poisoned;
};


} //namespace cdr

#endif //CDR_STRUCT_HPP