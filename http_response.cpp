/*
 *  http_response.cpp
 *  sitemon
 *
 *  Created by Peter Pearson on 27/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "http_response.h"

HTTPResponse::HTTPResponse(bool storeHeader, bool storeBody) : contentSize(0), downloadSize(0), m_storeHeader(storeHeader), m_storeBody(storeBody),
															errorCode(HTTP_OK), responseCode(0), redirectCount(0), redirectTime(0.0),
															lookupTime(0.0), connectTime(0.0), dataStartTime(0.0), totalTime(0.0), m_thread(0)
{

}