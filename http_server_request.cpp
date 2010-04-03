#include <sstream>
#include <iostream>

#include "http_server_request.h"

HTTPServerRequest::HTTPServerRequest(const std::string &request) : m_request(request)
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
		return false;
	
	int pathStart = 4;
	
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
		const std::string &params = path.substr(nQuestionMark + 1);
		
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
				
				if (!name.empty())
				{
					m_aParams[name] = value;
				}
			}			
		}
	}
	
	return true;
}

void HTTPServerRequest::split(const std::string &str, std::vector<std::string> &tokens, const std::string &sep)
{
    int lastPos = str.find_first_not_of(sep, 0);
    int pos = str.find_first_of(sep, lastPos);
	
    while (lastPos != -1 || pos != -1)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(sep, pos);
        pos = str.find_first_of(sep, lastPos);
    }
}