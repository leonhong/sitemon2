#ifndef REQUEST_THREAD_H
#define REQUEST_THREAD_H

#include "utils/thread.h"

#include "script.h"
#include "http_engine.h"

struct RequestThreadData
{
	RequestThreadData(int thread, Script *pScript, int debugLevel = 0) : m_thread(thread), m_pScript(pScript), m_debugLevel(debugLevel) { }		
	
	Script *m_pScript;
	int m_thread;
	int m_debugLevel;
};

class RequestThread : public Thread
{
public:
	RequestThread(RequestThreadData *data);
	virtual ~RequestThread();

	virtual void run();
	
	std::vector<HTTPResponse> &getResponses() { return m_aResponses; }

protected:
	int m_threadID;
	int m_debugLevel;
	Script m_Script;
	std::vector<HTTPResponse> m_aResponses;
};

#endif
