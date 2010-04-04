#include "http_server.h"
#include "utils/socket.h"
#include "http_server_request_thread.h"

#include "utils/sqlite_db.h"

HTTPServer::HTTPServer(int port, const std::string &webContentPath, const std::string &dbPath) : m_port(port), m_webContentPath(webContentPath), m_dbPath(dbPath)
{
	
}

bool HTTPServer::start()
{
	Socket::initWinsocks();
	
	Socket mainSocket;
	mainSocket.create();
	
	if (!mainSocket.bind(m_port))
	{
		printf("Couldn't bind to port: %i\n", m_port);
		return false;
	}
	
	if (!mainSocket.listen(20))
	{
		printf("Couldn't listen for connections.\n");
		return false;
	}
	
	SQLiteDB *pMainDB = NULL;
	
	if (!m_dbPath.empty())
	{
		pMainDB = new SQLiteDB(m_dbPath);
	}
	
	while (true)
	{
		Socket *newSock = new Socket();
		if (mainSocket.accept(newSock))
		{
			HTTPServerRequestThread *newThread = new HTTPServerRequestThread(newSock, m_webContentPath);
			
			if (newThread)
			{
				newThread->setAutodestruct(true);
				newThread->start();
			}
			else
			{
				printf("Couldn't create new thread\n");
			}
		}
		else
		{
			printf("Couldn't accept connection\n");
		}
	}
	
	if (pMainDB)
		delete pMainDB;
	
	Socket::cleanupWinsocks();
	return true;
}

