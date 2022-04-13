#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include<vector>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include"cdr_struct.hpp"

namespace cdr
{

int PackLine(std::string a_line, void* a_buffer);
std::string UnpackLine(void* a_buffer);


} //namespace cdr

#endif  //PROTOCOL_HPP