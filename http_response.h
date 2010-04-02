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

class HTTPResponse
{
public:
	HTTPResponse(bool storeHeader = true, bool storeBody = true);
	
	time_t		timestamp;
	long		responseCode;
	long		errorCode;
	double		lookupTime;
	double		connectTime;
	double		dataStartTime;
	double		totalTime;
	double		redirectTime;
	long		redirectCount;
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