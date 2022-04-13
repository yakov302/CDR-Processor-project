#include"tcp_client.hpp"

namespace tcp
{

TcpClient::TcpClient(const char* a_serverIp, int a_servrPort)
: m_socket(0)
, m_sin()
{
    memset (&m_sin, 0, sizeof(*&m_sin));
	m_sin.sin_family = AF_INET;
	m_sin.sin_addr.s_addr =  inet_addr(a_serverIp);
	m_sin.sin_port = htons(a_servrPort);

	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_socket < 0)
	{
		perror("Socket fail!\n");
		throw;
	}

	int optval = 1;	
	if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("Setsockopt fail!\n");
		throw;
	}

    if(connect(m_socket, (struct sockaddr*)&m_sin, sizeof(*&m_sin)) < 0)
	{	
		perror("Connect to server fail!\n");
		throw;	
	}
}

TcpClient::~TcpClient()
{
    close(m_socket);
}

bool TcpClient::sendAndReceiveData(void* a_buffer, int a_length)
{
	int sentByte = send(m_socket, a_buffer, a_length, 0);
	if(sentByte < 0)
	{
		perror("Send fail!\n");	
		return false;	
	}

	return true;
}


}//namespace tcp