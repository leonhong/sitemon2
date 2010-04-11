#include <sstream>
#include <iostream>

#include "http_server_request.h"
#include "utils/string_helper.h"

HTTPServerRequest::HTTPServerRequest(const std::string &request) : m_request(request), m_post(false)
{
	
}

bool HTTPServerRequest::parse()
{
	std::vector<std::string> lines;
	split(m_request, lines);
	
	if (lines.empty())
		return false;
	
	// first line should contain main request
	std::string &line = lines[0];
	
	if (line.find("HTTP/") == -1)
		return false;
	
	// only support GET requests for the moment
	if (line.substr(0, 3) != "GET")
	{
		if (line.substr(0, 4) != "POST")
			return false;
		
		m_post = true;
	}
	
	int pathStart = m_post ? 5 : 4;
	
	int pathEnd = line.rfind(" "); // should be the space before the HTTP version
	
	if (pathEnd == -1)
		return false;
	
	const std::string &path = line.substr(pathStart, pathEnd - pathStart);
	
	int nQuestionMark = path.find("?");
	if (nQuestionMark == -1)
	{
		m_path = path;
	}
	else
	{
		m_path = path.substr(0, nQuestionMark);
		
		// if POST has this on the end, ignore the URL params
		if (!m_post)
		{
			const std::string &params = path.substr(nQuestionMark + 1);
		
			addParams(params);
		}
	}
	
	if (m_post && lines.size() > 1)
	{
		// hopefully, last line of HTTP request should be the POST params
		
		std::string &params = lines.back();
		
		addParams(params);
	}
	
	return true;
}

void HTTPServerRequest::addParams(const std::string &params)
{
	std::vector<std::string> items;
	split(params, items, "&");
	
	for (std::vector<std::string>::iterator it = items.begin(); it != items.end(); ++it)
	{
		const std::string &item = *it;
		
		int sep = item.find("=");
		if (sep != -1)
		{
			std::string name = item.substr(0, sep);
			std::string value = item.substr(sep + 1);
			
			// convert hex encoding strings to native strings
			
			int nHex = 0;
			while ((nHex = value.find("%", nHex)) != -1)
			{
				std::string strHex = "0x" + value.substr(nHex + 1, 2);
				char cChar = strtol(strHex.c_str(), 0, 16);
				char szTemp[2];
				memset(szTemp, 0, 2);
				sprintf(szTemp, "%c", cChar);
				std::string strChar(szTemp);
				
				value.replace(nHex, 3, strChar);
			}

			// + to spaces

			int nSpace = 0;
			while ((nSpace = value.find("+", nSpace)) != -1)
			{
				value.replace(nSpace, 1, " ");
			}
			
			if (!name.empty())
			{
				m_aParams[name] = value;
			}
		}
	}
}