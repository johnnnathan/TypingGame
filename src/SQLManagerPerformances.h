#ifndef SQL_MANAGER_H
#define SQL_MANAGER_H

#include <sqlite3.h>
#include <string>
#include "performance.h"

int ManagerPerformancesMain(Performance *performance);
int createTable(sqlite3* DB);
int insertData(sqlite3* DB, Performance *performance);
int printDB(sqlite3* DB);
int callback(void* data, int argc, char** argv, char** azColName);

#endif // !SQL_MANAGER_H
