
#ifndef HTTP_SERVER_DB_HELPERS_H
#define HTTP_SERVER_DB_HELPERS_H

#include "http_engine.h"
#include "utils/sqlite_query.h"

bool createNeededTables(SQLiteDB *pDB);
bool createSingleTestHistoryTable(SQLiteDB *pDB);
bool addResponseToSingleTestHistoryTable(SQLiteDB *pDB, HTTPResponse &response);

bool getSingleTestHistoryList(SQLiteDB *pDB, std::string &output, int offset = 0);
bool formatDBSingleTestResponseToHTMLDL(SQLiteDB *pDB, long rowID, std::string &output);

#endif
