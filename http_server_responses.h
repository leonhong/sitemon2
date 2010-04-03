
#ifndef HTTP_SERVER_RESPONSES_H
#define HTTP_SERVER_RESPONSES_H

#include <string>
#include <iostream>
#include <sstream>

class HTTPServerResponse
{
public:
	HTTPServerResponse(int returnCode, const std::string &text = "");
	
	std::string responseString();
	
protected:
	int m_returnCode;
	std::string m_text;
};

#endif
