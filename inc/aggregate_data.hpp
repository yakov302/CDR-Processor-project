#ifndef AGGREGATE_DATA_HPP
#define AGGREGATE_DATA_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <utility>

namespace cdr
{

class AggData
{
public:
    AggData()
    : m_msisdn()
    , m_voiceOut(0)
    , m_voiceIn(0)
    , m_dataOut(0)
    , m_dataIn(0)
    , m_smsOut(0)
    , m_smsIn(0)
    , m_voiceLinks()
    , m_smsLinks(){}

    std::string m_msisdn;
    size_t m_voiceOut;
    size_t m_voiceIn;
    size_t m_dataOut;
    size_t m_dataIn;
    size_t m_smsOut;
    size_t m_smsIn;
    std::unordered_map <std::string, size_t> m_voiceLinks;
    std::unordered_map <std::string, size_t> m_smsLinks;
};

class OpAggData
{
public:
    OpAggData()
    : m_mccMnc(), m_voiceOut(0), m_voiceIn(0), m_smsOut(0), m_smsIn(0) {}

    std::string m_mccMnc;
    size_t m_voiceOut;
    size_t m_voiceIn;
    size_t m_smsOut;
    size_t m_smsIn;
};


} //namespace cdr

#endif //AGGREGATE_DATA_HPP