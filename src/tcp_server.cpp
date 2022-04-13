#include"tcp_server.hpp"

namespace tcp
{

namespace impel
{

cdr::UsageType returnEnum( std::string a_usageType)
{
    if(a_usageType == "MOC")
    {
        return cdr::MOC;
    }
    if(a_usageType == "MTC")
    {
        return cdr::MTC;
    }
    if(a_usageType == "SMS-O")
    {
        return cdr::SMS_MO;
    }
    if(a_usageType == "SMS-MT")
    {
        return cdr::SMS_MT;
    }
    if(a_usageType == "D")
    {
        return cdr::D;
    }
    if(a_usageType == "U")
    {
        return cdr::U;
    }
    if(a_usageType == "B")
    {
        return cdr::B;
    }

    return cdr::X;
}

cdr::Cdr makeCdr(std::string a_line)
{
    cdr::Cdr cdr;

    size_t begine = 0;
    size_t end = a_line.find('|', begine);
    cdr.m_sequenceNumber = stoi(a_line.substr(begine, end-begine));

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_imsi = a_line.substr(begine, end-begine);
    
    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_imei = a_line.substr(begine, end-begine);

    begine = end + 1;
    end = a_line.find('|', begine);
    std::string usageType (a_line.substr(begine, end-begine));
    cdr.m_usageType = returnEnum(a_line.substr(begine, end-begine));

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_msisdn = a_line.substr(begine, end-begine);

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_callDate = a_line.substr(begine, end-begine);

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_callTime = a_line.substr(begine, end-begine);

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_duration = stoi(a_line.substr(begine, end-begine));

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_bytesReceived = stoi(a_line.substr(begine, end-begine));

    begine = end + 1;
    end = a_line.find('|', begine);
    cdr.m_bytesTransmitted = stoi(a_line.substr(begine, end-begine));

    if(cdr.m_usageType != cdr::D)
    {
        begine = end + 1;
        end = a_line.find('|', begine);
        cdr.m_secondPartyImsi = a_line.substr(begine, end-begine);

        begine = end + 1;
        end = a_line.find('\n', begine);
        cdr.m_secondPartyMsisdn = a_line.substr(begine, end-begine);
    }
    else
    {
        cdr.m_secondPartyImsi = "non";
        cdr.m_secondPartyMsisdn = "non";
    }
    
    return cdr;
}

static void* threadFunction(void* arg){
	TcpServer* srv = static_cast<TcpServer*> (arg);
	srv->runServer();
	return 0;
}


} //namespace impel

TcpServer::TcpServer(const char* a_serverIp, int a_servrPort, mt::BlockingQueue<cdr::Cdr>& a_queue)
: m_listenSocket()
, m_sin()
, m_clientSin()
, m_socketsList()
, m_activeSockets()
, m_clientsCountr(0)
, m_activity()
, m_buffer()
, m_stop(false)
, m_clientSockets()
, m_queue(a_queue)
{
    //Sin setting
    memset (&m_sin, 0, sizeof(m_sin));	
	m_sin.sin_family = AF_INET;
	m_sin.sin_addr.s_addr =  inet_addr(a_serverIp);
	m_sin.sin_port = htons(a_servrPort);

	//Open listen socket
	m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_listenSocket < 0)
	{
		perror("socket fail");
		closeClientSockets();
        throw;
	}

	//Open stop socket
	m_stopSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_stopSocket < 0)
	{
		perror("socket fail");
		closeClientSockets();
        throw;
	}

	//Enter to fd_set for select	
	FD_SET(m_listenSocket, &m_socketsList);
	FD_SET(m_stopSocket, &m_socketsList);	
	
	//Frees listen socket immediately
    int optval = 1;		
	if(setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt fail");
		closeClientSockets();
        throw;
	}

	//Set fix ip and port for server		
	if(bind(m_listenSocket, (struct sockaddr*) &m_sin, sizeof(m_sin)) < 0)
	{
        perror("bind fail");
		closeClientSockets();
        throw;
	}
	
	//Set listen socket only for listen	
	if(listen(m_listenSocket, 1000) < 0)
	{
		perror("listen fail");
		closeClientSockets();
        throw;
	}

	m_thread = new cpp::Thread(impel::threadFunction, this);
}

TcpServer::~TcpServer()
{
	m_thread->join();
	delete m_thread;
}

void TcpServer::runServer()
{
	FD_ZERO(&m_activeSockets);

	while(!m_stop)
	{			
		m_activeSockets = m_socketsList;	
		m_activity = select(1024, &m_activeSockets, 0, 0, 0);
		if((m_activity < 0) && (errno != EINTR))
		{
		    perror("select fail");
			closeClientSockets();
            throw;	
		}

		if(FD_ISSET(m_listenSocket, &m_activeSockets))
		{
			acceptNewClient();
			m_activity--;
		}
		takeCareExistsClients();
	}
}

void TcpServer::acceptNewClient()
{
	socklen_t clientSinLen = sizeof(&m_clientSin);
	int clientSocket = accept(m_listenSocket, (struct sockaddr*)& m_clientSin, &clientSinLen);
	if(clientSocket < 0)
	{
		perror("accept fail");
		closeClientSockets();
        throw;					
	}
	else if (m_clientsCountr < 1000)
	{					
		//Enter to fd_set for select		
		FD_SET(clientSocket, &m_socketsList);	
			
		//Enter socket to list
        m_clientSockets.push_back(clientSocket);			
		++m_clientsCountr;
		return;	
	}
    close(clientSocket);	
}

void TcpServer::takeCareExistsClients()
{
    auto it = m_clientSockets.begin();
    auto end = m_clientSockets.end();
		
	while((it != end) && (m_activity > 0)) 
	{	
        int clientSocket = *it;
		if(FD_ISSET(clientSocket, &m_activeSockets))
		{
			if(receiveData(clientSocket) == false)
			{
				FD_CLR(clientSocket, &m_socketsList);
				close(clientSocket);
				m_clientSockets.erase(it);
				m_clientsCountr--;
			}
			else
			{		
				++it;
			}
			m_activity--;
		}
		else
		{		
			++it;
		}	 
	} 
}

bool TcpServer::receiveData(int a_clientSocket)
{
	unsigned char* bufferPtr = m_buffer;
	int receiveBytes = 0;
	int messageLen = 0;

	do{
		receiveBytes = recv(a_clientSocket, bufferPtr, 1, 0);
		if(receiveBytes == -1)
	  	{
			perror("recv fail");
			return false;
		}
		messageLen = *bufferPtr;
		bufferPtr += receiveBytes;
	}while (receiveBytes != 1);
	
	do{
		receiveBytes = recv(a_clientSocket, bufferPtr, messageLen, 0);
		if(receiveBytes == -1)
	  	{
			perror("recv fail");
			return false;
		}
		messageLen -= receiveBytes;
		bufferPtr += receiveBytes;
	}while (messageLen != 0);
	
	std::string line = cdr::UnpackLine(m_buffer);
	cdr::Cdr cdr = impel::makeCdr(line);
	m_queue.enqueue(cdr);

    return true;
}

void TcpServer::stop()
{
    m_stop = true;
	cdr::Cdr poisoned;
	poisoned.poisoned = true;
	m_queue.enqueue(poisoned);
}

void TcpServer::closeClientSockets()
{		
	for(auto it : m_clientSockets)
	{	
		close(it);
	}
}


}//namespace tcp