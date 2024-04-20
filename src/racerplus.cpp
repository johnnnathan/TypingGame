//imports
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
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
const char* textsFile = "texts.db";
//function declaration
std::pair <int,std::string> getText(); 
std::stack<char>  initializeStack(string text);
void printStack(std::stack<char> &stack);
int gameLoop();
int race(std::stack<char> &stack, int ID);
//functions

//main method
int  main (int argc, char *argv[]) {
  bool first = true;
  char restart = 'n';
  std::cout << "USERNAME: ";
  std::cin >> name;
  std::cout << std::endl;
  while (restart == 'y' || first == true){
    int a = gameLoop();
    
    std::cout << "Do you want to play again? \n(Y)es\n(N)o\n";
    std::cin >> restart;
    restart = (char) tolower(restart);
    first = false;
  }
}

int gameLoop(){
  int ch;
  std::pair<int,std::string> rowPtr = getText();
  std::cout << "DEBUG: GOT ROW FROM DATABASE: " << rowPtr.first<<" " << rowPtr.second<<  std::endl;
  int textID = rowPtr.first;
  std::string text = rowPtr.second;
  std::cout << text << std::endl;
  std::stack<char> stack = initializeStack(text);
  printStack(stack);
  int accuracy = race(stack, textID);
  return 0;
}



//returns a random row from the texts SQL file
std::pair<int,std::string> getText(){
  std::pair<int,std::string> infoPair = getRandom();
  return infoPair;
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
  std::cout << time;
  performance.name = name;
  performance.speed = (words/time)*60;
  performance.textID = ID;
  ManagerPerformancesMain(&performance);
  return counter;
}


