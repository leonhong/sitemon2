
#ifndef HTTP_SERVER_REQUEST_H
#define HTTP_SERVER_REQUEST_H

#include <string>
#include <map>
#include <vector>

class HTTPServerRequest
{
public:
	HTTPServerRequest(const std::string &request);
	
	bool parse();
	void split(const std::string &str, std::vector<std::string> &lines, const std::string &sep = "\n");
	
	const std::string &getPath() const { return m_path; }
	const std::map<std::string, std::string> &getParams() const { return m_aParams; }
	
	bool hasParams() const { return !m_aParams.empty(); }
	std::string getParam(const std::string &name) { return m_aParams[name]; }
	
protected:
	std::string		m_request;
	
	std::string		m_path;
	std::map<std::string, std::string> m_aParams;	
	
};

#endif
