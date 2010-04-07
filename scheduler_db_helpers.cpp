
#include "scheduler_db_helpers.h"

bool createNeededSchedulerTables(SQLiteDB *pDB)
{
	createScheduledSingleTestsTable(pDB);
	createScheduledSingleTestResultsTable(pDB);
	
	return true;
}

bool createScheduledSingleTestsTable(SQLiteDB *pDB)
{
	std::string sql = "create table if not exists scheduled_single_tests (enabled integer, description string, url string, referer string, expected_phrase string, interval integer, accept_compressed integer)";
	
	if (pDB)
	{
		SQLiteQuery q(*pDB, true);
		
		return q.execute(sql);		
	}	
	
	return false;
}

bool createScheduledSingleTestResultsTable(SQLiteDB *pDB)
{
	std::string sql1 = "create table if not exists scheduled_single_test_results (test_id integer, run_time date, error_code integer, response_code integer, lookup_time double,"
					"connect_time double, data_start_time double, total_time double, redirect_count integer, content_size integer, download_size integer)";
	
	std::string sql2 = "create index if not exists single_test_results on scheduled_single_test_results(test_id)";
	
	if (pDB)
	{
		SQLiteQuery q(*pDB, true);
		
		bool ret1 = q.execute(sql1);
		bool ret2 = q.execute(sql2);
		
		return ret1 && ret2;
	}	
	
	return false;
}

bool getScheduledSingleTestsFromDB(SQLiteDB *pDB, std::vector<ScheduledItem> &items)
{
	if (!pDB)
		return false;
	
	std::string sql = "select rowid, description, url, interval, accept_compressed from scheduled_single_tests where enabled = 1";

	SQLiteQuery q(*pDB);

	time_t timeNow;
	time(&timeNow);
	
	q.getResult(sql);
	while (q.fetchNext())
	{
		long testID = q.getLong();
		std::string description = q.getString();
		std::string url = q.getString();
		long interval = q.getLong();
//		long acceptCompressed = q.getLong();

		if (description.empty())
			description = " ";

		if (url.empty())
			continue;

		ScheduledItem newItem(0, description, interval, timeNow);
		newItem.setTestType(SINGLE_TEST);
		newItem.setTestID(testID);

		items.push_back(newItem);
	}
	
	return true;
}
