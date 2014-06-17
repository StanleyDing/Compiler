#ifndef QUADRUPLESOUTPUT_H
#define QUADRUPLESOUTPUT_H

#include <stack>
#include <vector>

using namespace std;

struct quadRuple
{
    string op, arg1, arg2, result;
};
string int2str(int );
quadRuple quadRupleMaker(string, string, string, string);
void quadRuplesOutput(map<string, string> &l_map, vector<quadRuple> &QT);
void resultOutput(string, bool &, bool &, int &, int &, int &, int &, stack<string> &, stack<string> &, vector<quadRuple> &);

#endif
