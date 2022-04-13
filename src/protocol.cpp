#include "protocol.hpp"

namespace cdr
{

int PackLine(std::string a_line, void* a_buffer)
{
    char* pack = static_cast<char*> (a_buffer);
    char* copy = (char*) a_line.c_str();
    int size = strlen(copy);
	*pack = size;
	for(int i = 1; i < size + 1; ++i)
	{
		*(pack + i) = *(copy + i - 1);
	}
	a_buffer = pack;
    return size + 1;
}

std::string UnpackLine(void* a_buffer)
{
    char* pack = static_cast<char*> (a_buffer);
    int lineSize = *pack;
    char copy[1000]; 
	strncpy(copy, pack, lineSize + 1); 
	*(copy + lineSize + 1) = '\0';
	std::string line((copy + 1), lineSize);
    return  line;
}


} //namespace cdr