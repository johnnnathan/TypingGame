//imports
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <stack>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <termio.h>
#include <chrono>
#include "SQLManagerPerformances.h"
#include "SQLManagerTexts.h"

//import simplification
using std::string;

//global variables
const string filename = "texts.txt";
std::string name;
sqlite3* DB;
const char* textsFile = "texts.db";
//function declaration
std::string* getText(); 
std::stack<char>  initializeStack(string text);
void printStack(std::stack<char> &stack);
int gameLoop();
int race(std::stack<char> &stack, int ID);
void decodeRow(const std::string& text, std::string* row);
//functions

//main method
int  main (int argc, char *argv[]) {
  bool first = true;
  char restart;
  std::cout << "USERNAME: ";
  std::cin >> name;
  while (restart == 'y' || first == true){
    int a = gameLoop();
    
    std::cout << "Do you want to play again? \n(Y)es\n(N)o\n";
    std::cin >> restart;
    restart = (char) tolower(restart);
    first = false;
  }
}
int initializeTextsDB(){
  sqlite3* DB;
  openTable(DB);
  TcreateTable(DB);
  return 0;
}


int gameLoop(){
  int ch;
  std::string* rowPtr = getText();
  int textID = std::stoi(rowPtr[0]);
  std::string text = rowPtr[1];
  std::stack<char> stack = initializeStack(text);
  printStack(stack);
  int accuracy = race(stack, textID);
  return 0;
}



//returns a random row from the texts SQL file
std::string* getText(){
  std::string text = returnRandom(DB);
  std::string* row = new std::string[2];
  decodeRow(text, row);
  return row;
}

void decodeRow(const std::string& text, std::string* row){
  size_t pos = text.find(',');
  row[0] = text.substr(0,pos);
  row[1] = text.substr(pos + 1);
}
std::stack<char> initializeStack(string text){
  std::stack<char> stack;
  for (int i = text.length() - 1; i > -1; i--){
    stack.push(text[i]);
  }
  return stack;
}

void printStack(std::stack<char> &stack){
  std::stack<char> copy = stack;
  while (!copy.empty()){
    std::cout << copy.top();
    copy.pop();
  }
  std::cout << std::endl;
}



int race(std::stack<char> &stack, int ID) {
  int counter = 0;
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  int ch;
  auto start = std::chrono::steady_clock::now();
  int words = 1;
  while (!stack.empty()) {
    
    ch = getchar();
    if (ch == EOF) break;
    if (ch == stack.top()) {
      if (ch == ' '){
        words += 1;
      }
      std::cout << "\033[1;32m" <<static_cast<char>(ch) << "\033[0m";
      stack.pop();
      ++counter;
    }
    else{
      std::cout << "\033[1;31m" << static_cast<char>(ch) << "\033[0m";
  }
  }
  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  Performance performance;
  int time = static_cast<int>(duration.count());
  performance.name = name;
  performance.speed = (words/time)*60;
  performance.textID = ID;
  ManagerPerformancesMain(&performance);
  return counter;
}


