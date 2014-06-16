using namespace std;

#ifndef QUADRUPLESOUTPUT_H
#define QUADRUPLESOUTPUT_H
struct quadRuple
{
    string op, arg1, arg2, result;
};
string int2str(int );
void quadRuplesOutput();
quadRuple quadRupleMaker(string, string, string, string);
void resultOutput(string, bool &, bool &, int &, int &, int &, int &, stack<string> &, stack<string> &, vector<quadRuple> &);
#endif
