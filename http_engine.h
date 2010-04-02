/*
 *  http_engine.h
 *  sitemon
 *
 *  Created by Peter Pearson on 27/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HTTP_ENGINE_H
#define HTTP_ENGINE_H

#include <curl/curl.h>

#include "http_request.h"
#include "http_response.h"

static size_t writeBodyData(void *buffer, size_t size, size_t nmemb, void *userp);
static size_t writeHeaderData(void *buffer, size_t size, size_t nmemb, void *userp);

static std::string buildParametersString(HTTPRequest &request);

class HTTPEngine
{
public:
	HTTPEngine(bool threaded = false);
	
	bool setupCURLHandleFromRequest(CURL *handle, HTTPRequest &request);
	bool performRequest(HTTPRequest &request, HTTPResponse &response, bool allowDebug = true);

	bool extractResponseFromCURLHandle(CURL *handle, HTTPResponse &response);
	
	void initCURLHandle();
	
protected:
	CURL *	m_handle;	
	
};

#endif