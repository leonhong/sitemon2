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
	HTTPResponse() { };
	
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
	long		downloadSize;
	std::string		contentType;
	std::string		contentEncoding;
	std::string		server;

};

#endif