#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <string>
using namespace std;

void lexer_map_init(map<string, string> &l_map); //initialize the lexer map
char lexer(map<string, string> &l_map, string str); //check the type of input symbol
bool isKOS(map<string, string> &l_map, string str); //keyword, operator, special words
bool isDigit(string str); //numbers
bool isInentifier(string str); //iidentifiers
bool isChar(string str); //char

#endif
