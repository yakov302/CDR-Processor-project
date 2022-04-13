#include "http_server.hpp"
#include "thread.hpp"
#include <unordered_map>
#include <string>
#include <stdio.h>

namespace http
{

namespace imple
{

class Arg
{
public:
    Arg(crow::SimpleApp& app)
    : m_app(app)
    {}
    
public:
    crow::SimpleApp& m_app;
};

inline void* innerRun(void* a_arg)
{
    http::HttpServer* arg = static_cast<http::HttpServer*>(a_arg);
    arg->run();

    return nullptr;
}


} // imple

inline HttpServer::HttpServer(cdr::DataBase& a_dataBase)
: m_app()
, m_prossor(a_dataBase)
{
    m_thread = new cpp::Thread(imple::innerRun, this);
    build();
}

inline void HttpServer::build()
{
    CROW_ROUTE(m_app,"/query/msisdn/<string>")
    ([this](std::string msisdn)
    {
        crow::json::wvalue x({{"msisdn", msisdn}});
        cdr::BilingData data;
        data = this->m_prossor.biling(msisdn);

        x["Voice out"] = data.m_voiceOut;
        x["Voice in"] = data.m_voiceIn;
        x["Sms out"] = data.m_smsOut;
        x["Sms in"] = data.m_smsIn;
        x["Data out"] = data.m_dataOut;
        x["Data in"] = data.m_dataIn;

        return x; 
    });

    CROW_ROUTE(m_app,"/query/link/msisdn/<string>")
    ([this](std::string msisdn)
    {
        crow::json::wvalue x({{"msisdn", msisdn}});
        cdr::BilingData data;
        data = this->m_prossor.biling(msisdn);
    
        for(auto i :  data.m_voiceLinks)
        {
            char c[50];
            const char* spMsisdn = i.first.c_str();
            sprintf(c, "Second party %s voice", spMsisdn);
            x[c] = i.second;
        }

        for(auto i :  data.m_smsLinks)
        {
            char c[50];
            const char* spMsisdn = i.first.c_str();
            sprintf(c, "Second party %s sms", spMsisdn);
            x[c] = i.second;
        }

        return x; 
    });

    CROW_ROUTE(m_app,"/query/operator/<string>")
    ([this](std::string mcc)
    {
        crow::json::wvalue x({{"MCC/MNC", mcc}});
        cdr::OpAggData data;
        data = this->m_prossor.operatorData(mcc);

        x["Voice out"] = data.m_voiceOut;
        x["Voice in"] = data.m_voiceIn;
        x["Sms out"] = data.m_smsOut;
        x["Sms in"] = data.m_smsIn;

        return x;
    });
}

inline void HttpServer::stop()
{
    m_app.stop();
    m_thread->join();
    delete m_thread;
}

inline void HttpServer::run()
{
   m_app.port(8080).run_async();
}


} // namespace http