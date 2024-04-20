#ifndef SQL_MANAGER_TEXTS_H
#define SQL_MANAGER_TEXTS_H

#include <cstdio>
#include <ostream>
#include <sqlite3.h>
#include <iostream>


int addTable();
std::pair<int,std::string> getRandom();
int openTable(sqlite3* DB);
int TcreateTable(sqlite3* DB);
int insertData(sqlite3* DB, const std::string &ttba);
int TprintDB(sqlite3* DB);
int Tcallback(void* data, int argc, char** argv, char** azColName); 
std::pair<int, std::string> returnRandom(sqlite3* DB);

#endif // !SQL_MANAGER_TEXTS_
