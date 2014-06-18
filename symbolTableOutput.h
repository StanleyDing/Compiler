#ifndef SYMBOLUNIT_H
#define SYMBOLUNIT_H

#include <string>
#include <vector>
#include <map>
using namespace std;

struct symbolUnit
{
    string symbol, token, type;
    int scope, vol;
};

void symbolTableOutput(map<string, string> &l_map, vector<symbolUnit> &ST); //output the symbol table

#endif
