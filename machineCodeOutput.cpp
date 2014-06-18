#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
#include "lexer.h"
#include "symbolTableOutput.h"
#include "quadRuplesOutput.h"
#include "machineCodeOutput.h"
using namespace std;

void machineCodeOutput(vector<symbolUnit> &ST, vector<quadRuple> &QT)
{
    int memuse = 0, mline = -1, line = -1, cib = 0, tl = -1, cib2 = 0, tl2 = -1, cond; //count the current line of quadruples, cib is code in the block, tl is the target line of jmp and jfalse
    bool isCond = false;
    vector<string> MC;
    vector<string>::iterator its;
    vector<quadRuple>::iterator it;
    string output;
    quadRuple Q;
    map<string, int> mem; //memory usage of each variables
    map<string, int>::iterator itr;
    map<string, string> arrupdate;
    map<string, string>::iterator ita;

    ofstream ofs;
    ofs.open("code.tm", ofstream::out);

    for(it = QT.begin(); it != QT.end(); ++it)
    {
	Q = *it; 
	if(Q.op != "[]=")
	{
	    if(mem.find(Q.arg1) == mem.end() && !isDigit(Q.arg1))
		mem.insert(pair<string, int>(Q.arg1, memuse++));
	    if(mem.find(Q.arg2) == mem.end() && Q.arg2 != "" && !isDigit(Q.arg2))
		mem.insert(pair<string, int>(Q.arg2, memuse++));
	    if(mem.find(Q.result) == mem.end() && !isDigit(Q.result))
		mem.insert(pair<string, int>(Q.result, memuse++));
	}
	else
	{
	    vector<symbolUnit>::iterator ita;
	    symbolUnit A;
	    for(ita = ST.begin(); ita != ST.end(); ++ita)
	    {
		A = *ita;
		if(A.symbol == Q.arg1 && mem.find(Q.arg1) == mem.end())
		{
		    mem.insert(pair<string, int>(Q.arg1, memuse));
		    memuse += A.vol;
		    break;
		}
	    }
	}
    }

    output = "LDC 6,0(0)";
    addstring(output, MC);
    for(it = QT.begin(); it != QT.end(); ++it)
    {
	line++;
	output.clear();
	Q = *it;

	if(tl == line)
	{
	    MC.insert(MC.end() - cib + 1, "JEQ 2," + int2str(MC.size() + 2) + "(6)");

	    cib = 0;
	    tl = -1;
	}

	if(tl2 == line)
	{
	    output = "LDC 0, 0(0)";
	    MC.insert(MC.end() - cib2, "JEQ 6," + int2str(MC.size() + 1) + "(6)");
	    cib2 = 0;
	    tl2 = -1;
	}

	if(Q.op == "=")
	{
	    if(isDigit(Q.arg1))
	    {
		output = "LDC 0," + int2str(atoi(Q.arg1.c_str())) + "(6)";
		addstring(output, MC);
		output = "ST 0," + int2str(mem.find(Q.result)->second) + "(6)";
		addstring(output, MC);
	    }
	    else
	    {
		output = "LD 0," + int2str(mem.find(Q.arg1)->second) + "(6)";
		addstring(output, MC);
		output = "ST 0," + int2str(mem.find(Q.result)->second) + "(6)";
		addstring(output, MC);
	    }
	    for(ita = arrupdate.begin(); ita != arrupdate.end(); ++ita)
	    {
		if(ita->first == Q.result)
		{
		    output = "ST 0," + ita->second + "(6)";
		    addstring(output, MC);
		    if(isCond) cib ++, cib2 ++;
		}
	    }

	    if(isCond) cib += 2, cib2 += 2;
	}
	else if(Q.op == "+" || Q.op == "-" || Q.op == "*" || Q.op == "/")
	{
	    if(!isDigit(Q.arg1))
		output = "LD 0," + int2str(mem.find(Q.arg1)->second) + "(6)";
	    else
		output = "LDC 0," + int2str(atoi(Q.arg1.c_str())) + "(6)";
	    addstring(output, MC);
	    if(!isDigit(Q.arg2))
		output = "LD 1," + int2str(mem.find(Q.arg2)->second) + "(6)";
	    else
		output = "LDC 1," + int2str(atoi(Q.arg2.c_str())) + "(6)";
	    addstring(output, MC);

	    if(Q.op == "+") output += "ADD ";
	    else if(Q.op == "-") output += "SUB ";
	    else if(Q.op == "*") output += "MUL ";
	    else if(Q.op == "/") output += "DIV ";
	    output += "2,0,1";
	    addstring(output, MC);
	    output = "ST 2," + int2str(mem.find(Q.result)->second) + "(6)";
	    addstring(output, MC);
	    for(ita = arrupdate.begin(); ita != arrupdate.end(); ++ita)
	    {
		if(ita->first == Q.result)
		{
		    output = "ST 0," + ita->second + "(6)";
		    addstring(output, MC);
		    if(isCond) cib ++, cib2 ++;
		}
	    }
	    if(isCond) cib += 4, cib2 += 4;
	}
	else if(Q.op == "<" || Q.op == "<=" || Q.op == ">" || Q.op == ">=" || Q.op == "==" || Q.op == "!=")
	{
	    isCond = true;
	    cib = 0;
	    output = "LD 0," + int2str(mem.find(Q.arg1)->second) + "(6)";
	    addstring(output, MC);
	    output = "LD 1," + int2str(mem.find(Q.arg2)->second) + "(6)";
	    addstring(output, MC);
	    output = "LDC 2,0(0)";
	    addstring(output, MC);
	    if(Q.op == "<") output = "LT";
	    else if(Q.op == "<=") output = "LE";
	    else if(Q.op == ">") output = "GT";
	    else if(Q.op == ">=") output = "GE";
	    else if(Q.op == "==") output = "EQ";
	    else if(Q.op == "!=") output = "NE";
	    output += " 2,0,1";
	    addstring(output, MC);
	    cond = MC.size() - 1;
	}
	else if(Q.op == "jfalse")
	{
	    tl = atoi(Q.arg1.c_str());
	    cib = 0;
	}
	else if(Q.op == "jmp")
	{
	    cib++;
	    cib2 = 0;
	    tl2 = atoi(Q.arg1.c_str());
	    if(tl2 < line)
	    {
		MC.insert(MC.end(), "JEQ 6," + int2str(cond) + "(6)");
		tl2 = -1;
	    }
		
	}
	else if(Q.op == "ret")
	{
	    output = "LD 0," + int2str(mem.find(Q.arg1)->second) + "(6)";
	    addstring(output, MC);
	    output = "OUT 0,0,0";
	    addstring(output, MC);
	}
	else if(Q.op == "[]=")
	{
	    if(!isDigit(Q.arg2))
	    {
		output = "LD 1," + int2str(mem.find(Q.arg1)->second + mem.find(Q.arg2)->second) + "(6)";
		addstring(output, MC);
		arrupdate.insert(pair<string, string>(Q.result, int2str(mem.find(Q.arg1)->second + mem.find(Q.arg2)->second)));
	    }
	    else
	    {
		output = "LD 1," + int2str(atoi(Q.arg1.c_str()) + atoi(Q.arg2.c_str())) + "(6)";
		addstring(output, MC);
		arrupdate.insert(pair<string, string>(Q.result, int2str(atoi(Q.arg1.c_str()) + atoi(Q.arg2.c_str()))));
	    }
	    output = "ST 1," + int2str(mem.find(Q.result)->second) + "(6)";
	    addstring(output, MC);
	    if(isCond) cib += 2, cib2 += 2;
	}
    }
    output = "HALT 1,0,0";
    addstring(output, MC);

    line = 0;
    for(its = MC.begin(); its != MC.end(); ++its)
	ofs<<line++<<": "<<*its<<endl;

    for(itr = mem.begin(); itr != mem.end(); ++itr)
	cout<<itr->first<<" "<<itr->second<<endl;

    ofs.close();
}

void addstring(string &s, vector<string> &MC)
{
    MC.push_back(s);
    s.clear();
}
