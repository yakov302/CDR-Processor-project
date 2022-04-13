#include "server_manager.hpp"

namespace cdr 
{

ServerManager::ServerManager(const char* a_ip, int a_port, int q_size)
: m_dataBase()
, m_queue(q_size)
, m_server(a_ip, a_port, m_queue)
, m_aggregator(m_dataBase, m_queue)
, m_httpServer(m_dataBase)
{

}

void ServerManager::print()const
{
    m_dataBase.printDataBaseByMsisdn();
    m_dataBase.printDataBaseByNmc();
}

void ServerManager::stop()
{
    m_server.stop();
    m_aggregator.stop();
    m_httpServer.stop();
}


}//namespace cdr 