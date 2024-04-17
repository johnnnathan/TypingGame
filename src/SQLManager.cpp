#include <cstdio>
#include <ostream>
#include <sqlite3.h>
#include <iostream>


int createTable(sqlite3* DB);
int insertData(sqlite3* DB, const std::string &ttba);
int printDB(sqlite3* DB);
int callback(void* data, int argc, char** argv, char** azColName); 



int main (int argc, char *argv[]) {
  
  sqlite3* DB;
  const char *filename = "texts.db";
  int rc;
  char *ttba;

  rc = sqlite3_open(filename, &DB);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR OPENING DATABASE" << sqlite3_errmsg(DB);
    return rc;
  }
  createTable(DB);

  std::cout << "GIVE THE TEXT THAT YOU WANT TO ADD TO THE DATABASE: ";
  std::cin >> ttba;

  insertData(DB, ttba);

  printDB(DB);  

  return 0;
}

int createTable(sqlite3* DB){
  const char* createTableSQL = "CREATE TABLE IF NOT EXISTS Texts(id INTEGER PRIMARY KEY AUTOINCREMENT, text TEXT)";
  int rc = sqlite3_exec(DB, createTableSQL, nullptr,nullptr,nullptr); 

  if (rc != SQLITE_OK){
    std::cerr << "ERROR CREATING TABLE" << sqlite3_errmsg(DB) << std::endl;
    return rc;
  }
  return SQLITE_OK;
  
}

int insertData(sqlite3* DB, const std::string &ttba/*means text to be added*/){
  std::string insertSQL = "INSERT INTO Texts (text) VALUES ('" + ttba + "');";
  int rc = sqlite3_exec(DB, insertSQL.c_str(), nullptr,nullptr,nullptr);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR INSERTING DATA INTO TABLE" << sqlite3_errmsg(DB) << std::endl;
    return rc;
  }
  return SQLITE_OK;
}

int printDB(sqlite3* DB){
  const char* selectSQL = "SELECT * FROM Texts";
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
