#include"processor.hpp"

namespace cdr
{

Processor::Processor(DataBase& a_data)
: m_data(a_data)
{

}

BilingData Processor::biling(std::string a_msisdn)
{
    BilingData biling;
    AggData aggData;
    aggData = m_data.giveAggDataByMsisdn(a_msisdn);

    biling.m_voiceOut = aggData.m_voiceOut;
    biling.m_voiceIn = aggData.m_voiceIn;
    biling.m_dataOut = static_cast<double>(aggData.m_dataOut)/1000;
    biling.m_dataIn = static_cast<double>(aggData.m_dataIn)/1000;
    biling.m_smsOut = aggData.m_smsOut;
    biling.m_smsIn = aggData.m_smsIn;
    biling.m_voiceLinks = aggData.m_voiceLinks;
    biling.m_smsLinks = aggData.m_smsLinks;

    return biling;
}

OpAggData Processor::operatorData(std::string a_mccMnc)
{
    OpAggData opAggData;
    opAggData = m_data.giveAggDataByMnc(a_mccMnc);
    return opAggData;
}


}//namespace cdr