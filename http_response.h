/*
 *  http_response.h
 *  sitemon
 *
 *  Created by Peter Pearson on 27/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>

enum HTTPResponseError
{
	HTTP_OK = 0,
	HTTP_COULDNT_RESOLVE_HOST = -1,
	HTTP_COULDNT_CONNECT = -2,
	HTTP_TIMEOUT = -3,
	HTTP_RECV_ERROR = -4,
	HTTP_UNKNOWN_ERROR = -10
};

class HTTPResponse
{
public:
	HTTPResponse(bool storeHeader = true, bool storeBody = true);
	
	time_t		timestamp;

	HTTPResponseError errorCode;
	long		responseCode;

	std::string errorString;
	
	double		lookupTime;
	double		connectTime;
	double		dataStartTime;
	double		totalTime;

	double		redirectTime;
	long		redirectCount;
	
	std::string		requestedURL;
	std::string		finalURL;

	std::string		content;
	std::string		header;

	long		contentSize;
	long		downloadSize;

	std::string		contentType;
	std::string		contentEncoding;
	std::string		server;
	
	int		m_thread;
	bool	m_storeHeader;
	bool	m_storeBody;
};

#endif