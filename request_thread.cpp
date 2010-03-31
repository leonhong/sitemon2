#include "request_thread.h"

RequestThread::RequestThread(requestThreadData *data)
{
	m_Script.copyScript(data->pScript);
	m_threadID = data->thread;
	delete data;
}

RequestThread::~RequestThread()
{

}

void RequestThread::run()
{
	for (std::vector<HTTPRequest>::iterator it = m_Script.begin(); it != m_Script.end(); ++it)
	{
		HTTPRequest &request = *it;
		HTTPResponse response;

		printf("Starting thread %i...\n", m_threadID);

		if (m_Engine.performRequest(request, response))
		{
			
		}
	}

}
