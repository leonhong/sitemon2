/*
 *  results_storage.h
 *  sitemon
 *
 *  Created by Peter Pearson on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <map>

#include "http_response.h"

class ConcurrentHitResults
{
public:
	ConcurrentHitResults() { }
	
	void addResult(int step, HTTPResponse &response);
	void addResults(std::vector<HTTPResponse> &results);
	
	bool outputResultsToCSV(std::string path);
	
protected:
	std::map<int, std::vector<HTTPResponse> > m_aResults;	
};