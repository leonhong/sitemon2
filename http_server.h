/*
 *  http_server.h
 *  sitemon
 *
 *  Created by Peter Pearson on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string>

class HTTPServer
{
public:
	HTTPServer(int port = 8080, const std::string &webContentPath = "");
	
	bool start();
	
protected:
	int m_port;
	std::string m_webContentPath;

};

#endif
