
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

class HTTPServerFileResponse
{
public:
	HTTPServerFileResponse(const std::string &path, bool binary = false);
	
	std::string responseString();
	
protected:
	std::string m_path;
	bool		m_binary;
};

class HTTPServerTemplateFileResponse
{
public:
	HTTPServerTemplateFileResponse(const std::string &path, std::string &content);
	
	std::string responseString();
	
protected:
	std::string m_path;
	std::string &m_content;
};

#endif
