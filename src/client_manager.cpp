#include "client_manager.hpp"

namespace cdr
{

ClientManager::ClientManager(const char* a_ip, int a_port)
:m_client(a_ip, a_port)
, m_reader(m_client)
{

}

void ClientManager::stop()
{
    m_reader.stop();
}

}//namespace cdr
