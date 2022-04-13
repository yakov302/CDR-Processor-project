#include "reader.hpp"

namespace cdr
{

namespace impel
{

void* threadFunction(void* a_arg)
{
    Reader* ag = static_cast<Reader*>(a_arg);
    ag->readAndSend();
    return 0;
}


}//namespace impel

Reader::Reader(tcp::TcpClient& a_client)
: m_client(a_client)
, m_buffer(new char[1000])
, m_stop(false)
{
    m_thread = new cpp::Thread(impel::threadFunction, this);
}

Reader::~Reader()
{
    delete[]m_buffer;
}

void Reader::readAndSend(size_t a_numOfLine)
{
    while(!m_stop)
    {
        std::string path = "../../test/tcp_client/cdr_file";
        for(const auto& entry : std::filesystem::directory_iterator(path))
        {
            std::ifstream file(entry.path());
            std::string currentPath = entry.path();
            std::string fileName = currentPath.substr(30);
            std::string donePath = "../../test/tcp_client/done/" + fileName;
            rename(entry.path(), donePath);

            std::string line;
            getline(file, line);
            size_t count = 0;
            while(count < a_numOfLine)
            {
                getline(file, line);
                if(line.size() < 5) {break;}
                cdr::PackLine(line, m_buffer);
                m_client.sendAndReceiveData(m_buffer, line.size()+1);
                ++count;
            }
        }
    }
}

void Reader::stop()
{
    m_stop = true;
    m_thread->join();
    delete m_thread;
}

} //namespace cdr