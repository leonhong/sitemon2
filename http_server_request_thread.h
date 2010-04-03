
#ifndef HTTP_SERVER_REQUEST_THREAD_H
#define HTTP_SERVER_REQUEST_THREAD_H

#include "utils/thread.h"
#include "utils/Socket.h"

class HTTPServerRequestThread: public Thread
{
public:
	HTTPServerRequestThread(Socket *socket, const std::string &webContentPath = "");
	virtual ~HTTPServerRequestThread();
	
	virtual void run();
	
protected:
	Socket *	m_pSocket;
	std::string	m_webContentPath;
	
};
	
#endif
