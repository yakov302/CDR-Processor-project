#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include "tcp_client.hpp"
#include "reader.hpp"

namespace cdr
{

class ClientManager
{
public:
    ClientManager(const char* a_ip, int a_port);
    void stop();

private:
    tcp::TcpClient m_client;
    cdr::Reader m_reader;
};


} //namespace cdr

#endif//CLIENT_MANAGER_HPP