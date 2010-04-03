/*
 *  http_server_responses.cpp
 *  sitemon
 *
 *  Created by Peter Pearson on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "http_server_responses.h"

HTTPServerResponse::HTTPServerResponse(int returnCode, const std::string &text) : m_returnCode(returnCode), m_text(text)
{
	
}

std::string HTTPServerResponse::responseString()
{
	std::string response;
	
	char szTemp[64];
	memset(szTemp, 0, 64);
	
	sprintf(szTemp, "HTTP/1.1 %i \n", m_returnCode);
	response += szTemp;
	
	response += "Content-Type: text/html; charset=UTF-8\n";
	
	memset(szTemp, 0, 64);
	sprintf(szTemp, "Content-Length: %ld\n\n", m_text.size());
	response += szTemp;
	
	response += m_text + "\n";
	
	return response;
}