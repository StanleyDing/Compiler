#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <string>
using namespace std;

void lexer_map_init(map<string, string> &l_map);
char lexer(map<string, string> &l_map, string str);
bool isKOS(map<string, string> &l_map, string str);
bool isDigit(string str);
bool isInentifier(string str);
bool isChar(string str);

#endif
