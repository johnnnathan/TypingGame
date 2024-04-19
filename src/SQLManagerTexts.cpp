#include <cstdio>
#include <ostream>
#include <sqlite3.h>
#include <iostream>

int addTable();
int openTable(sqlite3* DB);
int TcreateTable(sqlite3* DB);
int insertData(sqlite3* DB, const std::string &ttba);
int TprintDB(sqlite3* DB);
int Tcallback(void* data, int argc, char** argv, char** azColName); 
std::string returnRandom(sqlite3* DB);

const char *filename = "texts.db";
int addTexts() {
  
  sqlite3* DB;
  int rc;
  char *ttba;

  rc = sqlite3_open(filename, &DB);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR OPENING DATABASE" << sqlite3_errmsg(DB);
    return rc;
  }
  TcreateTable(DB);

  std::cout << "GIVE THE TEXT THAT YOU WANT TO ADD TO THE DATABASE: ";
  std::cin >> ttba;

  insertData(DB, ttba);

  TprintDB(DB);  

  return 0;
}

int openTable(sqlite3* DB){
  int rc; 

  rc = sqlite3_open(filename, &DB);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR OPENING DATABASE" << sqlite3_errmsg(DB);
    return rc;

  }
  return 0;

}
int TcreateTable(sqlite3* DB){
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

int TprintDB(sqlite3* DB){
  const char* selectSQL = "SELECT * FROM Texts";
  int rc = sqlite3_exec(DB, selectSQL, Tcallback, nullptr, nullptr);
  if (rc != SQLITE_OK){
    std::cerr << "ERROR PRINTING DATABASE" << sqlite3_errmsg(DB);
  }
  return rc;
}

int Tcallback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << '\t';
    }
    std::cout << std::endl;
    return 0;
}

std::string returnRandom(sqlite3* DB){
  const char* sql = "SELECT * FROM Texts ORDER BY RANDOM() LIMIT 1";
  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(DB,sql,-1,&stmt,nullptr); 
  if (rc != SQLITE_OK){
    std::cerr << "SQL ERROR IN FUNCTION returnRandom";
    return "";
  }
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW){
    std::string returnText;
    const unsigned char* text = sqlite3_column_text(stmt, 0);
    int cols = sqlite3_column_count(stmt);
    for (int i = 0; i < cols; i++){
      const unsigned char* value = sqlite3_column_text(stmt,i);
      if (value){
        returnText += reinterpret_cast<const char*>(value);
        returnText += ",";
      }   

    }
    sqlite3_finalize(stmt);
    return returnText;
  }
  sqlite3_finalize(stmt);
  return "";
}
