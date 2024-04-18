#include <cstdio>
#include <ostream>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include "performance.h"

int createTable(sqlite3* DB);
int insertData(sqlite3* DB, Performance *performance);
int printDB(sqlite3* DB);
int callback(void* data, int argc, char** argv, char** azColName); 




int ManagerPerformancesMain(Performance *performance){
  sqlite3* DB;
  const char *filename = "races.db";
  int rc;
  char *ttba;

  rc = sqlite3_open(filename, &DB);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR OPENING DATABASE" << sqlite3_errmsg(DB);
    return rc;
  }
  createTable(DB);


  insertData(DB, performance);

  printDB(DB);  

  return 0;
}

int createTable(sqlite3* DB){
  const char* createTableSQL = "CREATE TABLE IF NOT EXISTS Performances(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, speed INTEGER, textid INTEGER)";
  int rc = sqlite3_exec(DB, createTableSQL, nullptr,nullptr,nullptr); 

  if (rc != SQLITE_OK){
    std::cerr << "ERROR CREATING TABLE" << sqlite3_errmsg(DB) << std::endl;
    return rc;
  }
  return SQLITE_OK;
  
}

int insertData(sqlite3* DB, Performance *performance/*means text to be added*/){
  
std::string insertSQL = "INSERT INTO Performances (name, speed, textid) VALUES ('" + 
                        performance->name + "', " + 
                        std::to_string(performance->speed) + ", " + 
                        std::to_string(performance->textID) + ");";

  int rc = sqlite3_exec(DB, insertSQL.c_str(), nullptr,nullptr,nullptr);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR INSERTING DATA INTO TABLE" << sqlite3_errmsg(DB) << std::endl;
    return rc;
  }
  return SQLITE_OK;
}

int printDB(sqlite3* DB){
  const char* selectSQL = "SELECT * FROM Performances";
  int rc = sqlite3_exec(DB, selectSQL, callback, nullptr, nullptr);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR PRINTING DATABASE" << sqlite3_errmsg(DB);
  }
  return rc;
}

int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << '\t';
    }
    std::cout << std::endl;
    return 0;
}
