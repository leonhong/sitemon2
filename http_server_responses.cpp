/*
 *  http_server_responses.cpp
 *  sitemon
 *
 *  Created by Peter Pearson on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <fstream>

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

HTTPServerFileResponse::HTTPServerFileResponse(const std::string &path, bool binary) : m_path(path), m_binary(binary)
{
	
}

std::string HTTPServerFileResponse::responseString()
{
	std::string response;

	std::fstream fileStream(m_path.c_str(), std::ios::in);
	
	std::string content;
	int returnCode = 200;
	
	if (fileStream.fail())
	{
		content = "File not found.\n";
		returnCode = 404;
	}
	else
	{
		std::string line;
		char buf[1024];
		
		while (fileStream.getline(buf, 1024))
		{
			line.assign(buf);
			content += line + "\n";		
		}
	}
	fileStream.close();
	
	char szTemp[64];
	memset(szTemp, 0, 64);
	
	sprintf(szTemp, "HTTP/1.1 %i \n", returnCode);
	response += szTemp;
	
	response += "Content-Type: text/html; charset=UTF-8\n";
	
	memset(szTemp, 0, 64);
	sprintf(szTemp, "Content-Length: %ld\n\n", content.size());
	response += szTemp;
	
	response += content + "\n";

	return response;
}
