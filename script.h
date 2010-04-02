/*
 *  script.h
 *  sitemon
 *
 *  Created by Peter Pearson on 28/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCRIPT_H
#define SCRIPT_H

#include <vector>

#include "utils/tinyxml.h"

#include "http_request.h"

class Script
{
public:
	Script() { }

	void copyScript(Script *pScript);
	
	bool loadScriptFile(const std::string &file);

	inline std::vector<HTTPRequest>::iterator begin() { return m_aSteps.begin(); }
	inline std::vector<HTTPRequest>::iterator end() { return m_aSteps.end(); }

	// functions to set stuff for all steps
	void setAcceptCompressed(bool acceptCompressed);

protected:
	void loadRequestElement(TiXmlElement *pElement);
	void loadParamsElement(TiXmlElement *pElement, HTTPRequest &request);
	void loadCookiesElement(TiXmlElement *pElement, HTTPRequest &request);	
	
protected:
	std::vector<HTTPRequest> m_aSteps;	
};


#endif