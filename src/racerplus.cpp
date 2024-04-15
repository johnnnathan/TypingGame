//imports
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stack>
#include <fstream>
#include <ctime>

//import simplification
using std::string;

//global variables
const string filename = "texts.txt";

//function declaration
int getTextsAmount();
string getText(int pos); 
std::stack<char>  initializeStack(string text);
void printStack(std::stack<char> &stack);

//functions

//main method
int  main (int argc, char *argv[]) {
  int ch;
  int fileLength = getTextsAmount();
  std::srand(std::time(nullptr));
  int textnr = (std::rand() % fileLength);
  string text = getText(textnr);
  
  std::stack<char> stack = initializeStack(text);
  printStack(stack);
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