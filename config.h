#ifndef CONFIG_H
#define CONFIG_H

#include "utils/tinyxml.h"

class Config
{
public:
	Config() { }
	
	bool loadConfigFile(const std::string &configFilePath = "");
	
	std::string	getWebContentPath() { return m_webContentPath; }
	
protected:
	std::string		m_configFilePath;
	std::string		m_webContentPath;	
	
};


#endif