/*
 *  http_server_db_helpers.cpp
 *  sitemon
 *
 *  Created by Peter Pearson on 04/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "http_server_db_helpers.h"
#include "http_server_html_formatters.h"

bool createNeededTables(SQLiteDB *pDB)
{
	createSingleTestHistoryTable(pDB);
	
	return true;
}

bool createSingleTestHistoryTable(SQLiteDB *pDB)
{
	std::string sql = "create table if not exists single_test_history (run_time date, url string, return_code integer, lookup_time double,"
					   "connect_time double, data_start_time double, total_time double, content_size integer, download_size integer)";
	
	if (pDB)
	{
		SQLiteQuery q(*pDB);
		
		return q.execute(sql);		
	}	
	
	return false;
}

bool addResponseToSingleTestHistoryTable(SQLiteDB *pDB, HTTPResponse &response)
{
	if (!pDB)
		return false;
	
	std::string sql = "insert into single_test_history values (datetime('now'), ";
	
	char szTemp[1024];
	memset(szTemp, 0, 1024);
	sprintf(szTemp, "'%s', %ld, %f, %f, %f, %f, %ld, %ld)", response.finalURL.c_str(), response.responseCode, response.lookupTime, response.connectTime,
			response.dataStartTime, response.totalTime, response.contentSize, response.downloadSize);
	
	sql.append(szTemp);
	
	SQLiteQuery q(*pDB);
	
	return q.execute(sql);
}

bool getSingleTestHistoryList(SQLiteDB *pDB, std::string &output, int offset)
{
	if (!pDB)
		return false;
	
	std::string sql = "select rowid, datetime(run_time,'localtime') as rtime, url, return_code, total_time, download_size,"
							"content_size from single_test_history limit 20 offset ";
	
	char szOffset[8];
	memset(szOffset, 0, 8);
	sprintf(szOffset, "%i", offset);
	sql.append(szOffset);
	
	SQLiteQuery q(*pDB);
	
	output = "";
	
	char szTemp[2048];
	
	q.getResult(sql);
	while (q.fetchNext())
	{
		memset(szTemp, 0, 2048);
		
		long runID = q.getLong();
		std::string time = q.getString();
		std::string url = q.getString();
		long returnCode = q.getLong();
		float totalTime = q.getDouble();
		long downloadSize = q.getLong();
		long contentSize = q.getLong();
		
		sprintf(szTemp, "<tr>\n <td><a href=\"/single_details?runid=%ld\">%ld</a></td>\n <td>%s</td>\n <td>%s</td>\n <td>%ld</td>\n <td>%f</td>\n <td>%ld</td>\n <td>%ld</td></tr>\n",
						runID, runID, time.c_str(), url.c_str(), returnCode, totalTime, downloadSize, contentSize);
		
		output.append(szTemp);		
	}
	
	return true;
}

bool formatDBSingleTestResponseToHTMLDL(SQLiteDB *pDB, long rowID, std::string &output)
{
	char szRowID[12];
	memset(szRowID, 0, 12);
	
	sprintf(szRowID, "%ld", rowID);
	
	std::string sql = "select datetime(run_time,'localtime') as rtime, url, return_code, lookup_time, connect_time, data_start_time, total_time, download_size,"
						"content_size from single_test_history where rowid = ";
	sql.append(szRowID);
	
	SQLiteQuery q(*pDB);
	
	output = "";
	
	q.getResult(sql);
	if (q.fetchNext())
	{
		char szTemp[2048];
		memset(szTemp, 0, 2048);
		
		std::string format = "<dl>\n";
		addStringToDL(format, "Time");
		addStringToDL(format, "Final URL");
		addLongToDL(format, "Response code");
		addFloatToDL(format, "Lookup time");
		addFloatToDL(format, "Connect time");
		addFloatToDL(format, "Data start time");
		addFloatToDL(format, "Total time");
		addLongToDL(format, "Download size");
		addLongToDL(format, "Content size");
		format += "</dl>\n";
		
		std::string time = q.getString();
		std::string url = q.getString();
		long responseCode = q.getLong();
		float lookupTime = q.getDouble();
		float connectTime = q.getDouble();
		float dataStartTime = q.getDouble();
		float totalTime = q.getDouble();
		long downloadSize = q.getLong();
		long contentSize = q.getLong();
		
		sprintf(szTemp, format.c_str(), time.c_str(), url.c_str(), responseCode, lookupTime, connectTime,
				dataStartTime, totalTime, contentSize, downloadSize);
		
		output.assign(szTemp);
		
		return true;
	}
	else
	{
		output = "Couldn't find requested runid in database.\n";
	}
	
	return false;
}