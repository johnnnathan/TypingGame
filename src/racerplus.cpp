//imports
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <stack>
#include <fstream>
#include <ctime>
#include <ncurses.h>
#include <unistd.h>
#include <termio.h>
#include <chrono>
#include "SQLManagerPerformances.h"


//import simplification
using std::string;

//global variables
const string filename = "texts.txt";
std::string name;
//function declaration
int getTextsAmount();
string getText(int pos); 
std::stack<char>  initializeStack(string text);
void printStack(std::stack<char> &stack);
int gameLoop();
int race(std::stack<char> &stack);
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

int gameLoop(){
  int ch;
  int fileLength = getTextsAmount();
  std::srand(std::time(nullptr));
  int textnr = (std::rand() % fileLength);
  string text = getText(textnr);
  
  std::stack<char> stack = initializeStack(text);
  printStack(stack);
  int accuracy = race(stack);
  return 0;
}


//get the length of the texts file, need to know to pick a random line from the file, return the length in int form
int getTextsAmount(){
  int ch;
  int fileLength = 0;
  std::ifstream infile(filename);

  if (!infile.is_open()){
    return 1;
  }
  std::string line;

  while (std::getline(infile,line)){
    ++fileLength;
  }

  infile.close();
  return fileLength;
}

//gets a text from the texts file using the position that is given to it, returns the content of the line 
string getText(int pos){
  std::ifstream infile(filename);
  string line;
  if (!infile.is_open()){
    return "";
  }
  for (int i = 0; i <= pos; i++){
    std::getline(infile, line); 
  }
  infile.close();
  return line;
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



int race(std::stack<char> &stack) {
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
        ++words;
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
  ManagerPerformancesMain(&performance);
  return counter;
}


