#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include "data_base.hpp"
#include "blocking_queue.hpp"
#include "tcp_server.hpp"
#include "aggregator.hpp"
#include "cdr_struct.hpp"
#include "http_server.hpp"

namespace cdr
{

class ServerManager
{
public:
    ServerManager(const char* a_ip, int a_port, int q_size = 1000);
    void print()const;
    void stop();

private:
    cdr::DataBase m_dataBase;
    mt::BlockingQueue<cdr::Cdr> m_queue;
    tcp::TcpServer m_server;
    cdr::Aggregator m_aggregator;
    http::HttpServer m_httpServer;
};


} //namespace cdr

#endif//SERVER_MANAGER_HPP