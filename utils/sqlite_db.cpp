/*
 *  sqlite_db.cpp
 *  sqlite
 *
 *  Created by Peter Pearson on 04/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "sqlite_db.h"

SQLiteDB::SQLiteDB(const std::string &path) : m_path(path), m_useMutex(false), m_mutex(m_mutex)
{
	
}

SQLiteDB::SQLiteDB(Mutex &mutex, const std::string &path) : m_mutex(mutex), m_useMutex(true), m_path(path)
{
	
}

SQLiteDB::~SQLiteDB()
{
	std::list<DBConn *>::iterator it = m_aAvailableConnections.begin();
	for (; it != m_aAvailableConnections.end(); ++it)
	{
		DBConn *conn = *it;
		
		sqlite3_close(conn->m_pDB);
		
		delete conn;
		
		// need to check return code from close was SQLITE_OK and empty the list
	}
}

DBConn *SQLiteDB::getDBConnection()
{
	if (m_useMutex)
	{
		m_mutex.lock();
	}
	
	DBConn * pConn = NULL;
	
	std::list<DBConn *>::iterator it = m_aAvailableConnections.begin();
	for (; it != m_aAvailableConnections.end(); ++it)
	{
		if (!(*it)->m_inUse)
		{
			pConn = *it;
			break;
		}
	}
	
	if (pConn)
	{
		pConn->m_inUse = true;
	}
	else
	{
		pConn = new DBConn;
		
		if (!pConn)
		{
			printf("Couldn't alloc memory for struct...\n");
			if (m_useMutex)
			{
				m_mutex.unlock();
			}
			return NULL;
		}
		
		int rc = sqlite3_open(m_path.c_str(), &pConn->m_pDB);
		if (rc)
		{
			printf("Can't open specified database file: %s\n", sqlite3_errmsg(pConn->m_pDB));
			sqlite3_close(pConn->m_pDB);
			delete pConn;
			if (m_useMutex)
			{
				m_mutex.unlock();
			}
			return NULL;
		}
		pConn->m_inUse = true;
		
		m_aAvailableConnections.push_back(pConn);
	}
	
	if (m_useMutex)
	{
		m_mutex.unlock();
	}
	
	return pConn;
}

void SQLiteDB::releaseDBConnection(DBConn *pConn)
{
	if (m_useMutex)
	{
		m_mutex.lock();
	}
	
	if (pConn)
	{
		pConn->m_inUse = false;
	}
}