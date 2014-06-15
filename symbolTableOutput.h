#ifndef SYMBOLUNIT_H
#define SYMBOLUNIT_H

#include <string>
#include <map>
using namespace std;

struct symbolUnit
{
    string symbol, token, type;
    int scope;
};

void symbolTableOutput(map<string, string> &l_map);

#endif
