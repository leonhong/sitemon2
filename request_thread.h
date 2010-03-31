#ifndef REQUEST_THREAD_H
#define REQUEST_THREAD_H

#include "utils/thread.h"

#include "script.h"
#include "http_engine.h"

struct requestThreadData
{
	Script *pScript;
	int thread;
};

class RequestThread : public Thread
{
public:
	RequestThread(requestThreadData *data);
	~RequestThread();

	virtual void run();

protected:
	int m_threadID;
	HTTPEngine m_Engine;
	Script m_Script;
};

#endif
