#ifndef BILING_DATA_HPP
#define BILING_DATA_HPP

#include <cstddef> 
#include <unordered_map>
#include <string>
#include "data.hpp"

namespace cdr 
{

struct BilingData : public Data
{
    BilingData() 
    : m_voiceOut(0)
    , m_voiceIn(0)
    , m_dataOut(0)
    , m_dataIn(0)
    , m_smsOut(0)
    , m_smsIn(0)
    , m_voiceLinks()
    , m_smsLinks(){}

    size_t m_voiceOut;
    size_t m_voiceIn;
    double m_dataOut;
    double m_dataIn;
    size_t m_smsOut;
    size_t m_smsIn;
    std::unordered_map <std::string, size_t> m_voiceLinks;
    std::unordered_map <std::string, size_t> m_smsLinks;
};


} // cdr

#endif // BILING_DATA_HPP