#ifndef READER_HPP
#define READER_HPP

#include <vector>
#include <fstream>
#include <filesystem>
#include <sys/file.h>
#include <iostream>
#include <unistd.h>

#include "cdr_struct.hpp"
#include "tcp_client.hpp"
#include "protocol.hpp"
#include "thread.hpp"

namespace cdr
{

class Reader
{
public:
    Reader(tcp::TcpClient& a_client);
    ~Reader();
    void stop();

    void readAndSend(size_t numOfLine = 100000);

private:
    Reader(Reader const& a_source);
    Reader& operator=(Reader const& a_source);

private:
    tcp::TcpClient& m_client;
    char* m_buffer;
    cpp::Thread* m_thread;
    bool m_stop;
};


} //namespace cdr

#endif  //READER_HPP