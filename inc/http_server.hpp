#ifndef HTTP_SERVER
#define HTTP_SERVER

#include "processor.hpp"
#include "data_base.hpp"
#include "crow.h"
#include "thread.hpp"
#include "biling_data.hpp"

namespace http
{

namespace imple
{
    class Arg;

} // namespace imple

class HttpServer
{
public:
    HttpServer(cdr::DataBase& a_dataBase);

    void build();
    void run();
    void stop();

private:
    crow::SimpleApp m_app;
    cdr::Processor m_prossor;
    cpp::Thread* m_thread;
};

} // namespace http

#include "./inl/http_server.hxx"

#endif //HTTP_SERVER