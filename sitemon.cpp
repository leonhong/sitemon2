#include "http_engine.h"
#include "sitemon.h"
#include "request_thread.h"

bool performSingleRequest(HTTPRequest &request)
{
	HTTPEngine engine;
	HTTPResponse response;

	if (engine.performRequest(request, response))
	{
		outputResponse(response);
	}
	else
	{
		return false;
	}

	return true;
}

bool performScriptRequest(Script &script)
{
	HTTPEngine engine;

	for (std::vector<HTTPRequest>::iterator it = script.begin(); it != script.end(); ++it)
	{
		HTTPRequest &request = *it;
		HTTPResponse response;

		std::cout << "Testing... " << request.getDescription() << "\n";

		if (engine.performRequest(request, response))
		{
			outputResponse(response);
		}
	}

	return true;
}

bool performConcurrentScriptRequest(Script &script, int threads, const std::string &outputPath)
{
#ifdef _MSC_VER
	thread_setup();
#endif

	std::vector<RequestThread *> aThreads;

	for (int i = 0; i < threads; i++)
	{
		requestThreadData *data = new requestThreadData;
		data->pScript = &script;
		data->thread = i + 1;
	
		RequestThread *newThread = new RequestThread(data);

		if (newThread)
		{
			newThread->start();

			aThreads.push_back(newThread);
		}
	}

	for (std::vector<RequestThread *>::iterator it = aThreads.begin(); it != aThreads.end(); ++it)
	{
		(*it)->waitForCompletion();
	}

#ifdef _MSC_VER
	thread_cleanup();
#endif

	return true;
}

void outputResponse(const HTTPResponse &response)
{
	std::cout << "Final URL: " << response.finalURL << "\n";
	std::cout << "Respone code: " << response.responseCode << "\n";

	std::cout << "DNS Lookup: " << response.lookupTime << " seconds.\n";
	std::cout << "Connection: " << response.connectTime << " seconds.\n";
	std::cout << "Data start: " << response.dataStartTime << " seconds.\n";

	if (response.redirectCount)
	{
		std::cout << "Redirect count: " << response.redirectCount << ".\n";
		std::cout << "Redirect time: " << response.redirectTime << " seconds.\n";
	}

	std::cout << "Total time: " << response.totalTime << " seconds.\n";
}

#ifdef _MSC_VER

void win32_locking_callback(int mode, int type, char *file, int line)
{
	if (mode & CRYPTO_LOCK)
	{
		WaitForSingleObject(lock_cs[type], INFINITE);
	}
	else
	{
		ReleaseMutex(lock_cs[type]);
	}
}

void thread_setup()
{
	int i;

	lock_cs = (void**)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(HANDLE));
	for (i = 0; i < CRYPTO_num_locks(); i++)
	{
		lock_cs[i] = CreateMutex(NULL, FALSE, NULL);
	}

	CRYPTO_set_locking_callback((void (*)(int, int, const char *, int))win32_locking_callback);
}

void thread_cleanup()
{
	int i;

	CRYPTO_set_locking_callback(NULL);
	for (i = 0; i < CRYPTO_num_locks(); i++)
		CloseHandle(lock_cs[i]);

	OPENSSL_free(lock_cs);
}

static unsigned long id_function(void)
{
  return ((unsigned long)GetCurrentThreadId());
}

#endif