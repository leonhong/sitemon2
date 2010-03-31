#include <iostream>
#include <curl/curl.h>

#include "script.h"

#include "sitemon.h"

int main(int argc, char *const argv[])
{
	char *szURL = 0;
	char *szScript = 0;

	bool isScript = false;
	bool concurrent = false;
	int threads = 0;

    curl_global_init(CURL_GLOBAL_ALL);

	if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0) || strcmp(argv[1], "/?") == 0))
	{
		// print usage
		std::cout << "Invalid usage\n";
		return 0;
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "-s") == 0 && i < argc)
			{
				isScript = true;
				szScript = (char *)argv[i++ + 1];
			}
			else if (strcmp(argv[i], "-sm") == 0 && i < argc)
			{
				isScript = true;
				concurrent = true;
				szScript = (char *)argv[i + 1];
				
				threads = atoi((char *)argv[i + 2]);
				
				i += 2;
			}
			else
			{
				szURL = (char *)argv[i];
			}
		}
	}
	
	if (!isScript)
	{
		HTTPRequest request(szURL);	
	
		performSingleRequest(request);
	}
	else
	{
		Script script;
		if (!script.loadScriptFile(szScript))
		{
			std::cout << "Can't open file...\n";
			return -1;
		}

		if (!concurrent)
		{
			performScriptRequest(script);
		}
		else
		{
			performConcurrentScriptRequest(script, threads, "");
		}
	}
	
    return 0;
}
