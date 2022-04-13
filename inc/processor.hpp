#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include<vector>
#include<fstream>
#include"data_base.hpp"
#include"biling_data.hpp"
#include"operator_data.hpp"
#include"data.hpp"

namespace cdr
{

class Processor
{
public:
    Processor(DataBase& a_data);
    BilingData biling(std::string a_msisdn);
    OpAggData operatorData(std::string a_mnc);

private:
    DataBase& m_data;
};


} //namespace cdr

#endif  //PROCESSOR_HPP