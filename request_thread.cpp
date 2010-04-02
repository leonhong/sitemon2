#include "request_thread.h"

RequestThread::RequestThread(RequestThreadData *data)
{
	m_Script.copyScript(data->m_pScript);
	m_threadID = data->m_thread;
	m_debugLevel = data->m_debugLevel;
	
	delete data;
}

RequestThread::~RequestThread()
{

}

void RequestThread::run()
{
	HTTPEngine engine(false);
	
	engine.initCURLHandle();
	
	for (std::vector<HTTPRequest>::iterator it = m_Script.begin(); it != m_Script.end(); ++it)
	{
		HTTPRequest &request = *it;
		
		HTTPResponse response(false, false); // don't store content
		response.m_thread = m_threadID;

		if (m_debugLevel > 0)
			printf("Starting thread %i...\n", m_threadID);

		if (engine.performRequest(request, response))
		{
			m_aResponses.push_back(response);
			
			if (m_debugLevel > 0)
				printf("Thread: %i\tOK\n", m_threadID);
		}
	}
}
