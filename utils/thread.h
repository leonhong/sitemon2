#ifndef THREAD_H
#define THREAD_H

#ifndef _MSC_VER
#include <pthread.h>
#else
#include <windows.h>
#endif

#include "Mutex.h"

class Thread
{
public:
	Thread();
	~Thread();

	bool start();
	void stop();
	void waitForCompletion();
	void setRunning(bool running);

	void setAutodestruct(bool autoDestruct) { m_autoDestruct = autoDestruct; }
	bool shouldAutodestruct() { return m_autoDestruct; }

	virtual void run() = 0;

public:
#ifdef _MSC_VER
	static unsigned long __stdcall threadProc(void *ptr);
#else
	friend void *threadProc(void *ptr);
#endif

#ifdef _MSC_VER
	HANDLE m_thread;
#else
	pthread_t m_thread;
#endif

	Thread *m_threadObject;
	bool m_isRunning;
	bool m_autoDestruct;
	Mutex m_mutex;
};

#endif
