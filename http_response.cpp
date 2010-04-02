/*
 *  http_response.cpp
 *  sitemon
 *
 *  Created by Peter Pearson on 27/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "http_response.h"

HTTPResponse::HTTPResponse(bool storeHeader, bool storeBody) : contentSize(0), downloadSize(0), m_storeHeader(storeHeader), m_storeBody(storeBody)
{

}