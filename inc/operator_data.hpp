#ifndef OPERATOR_DATA_HPP
#define OPERATOR_DATA_HPP

#include <cstddef> 
#include "data.hpp"

namespace cdr 
{

struct OperatorData : public Data {
    OperatorData() 
    : m_totalVoiceOut(0)
    , m_totalVoiceIn(0)
    , m_totalSmsOut(0)
    , m_totalSmsIn(0){}

    size_t m_totalVoiceOut;
    size_t m_totalVoiceIn;
    size_t m_totalSmsOut;
    size_t m_totalSmsIn;
};


} //namespace cdr 

#endif // OPERATOR_DATA_HPP
