#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <iomanip>
#include "lexer.h"
#include "quadRuplesOutput.h"

using namespace std;

void quadRuplesOutput(map<string, string> &l_map, vector<quadRuple> &QT)
{
    bool isBreak;
    string str;

    bool isNot, isMinus, isReturn; //if !, if -, if return
    int tv, code_in_block, ifValue, whileValue; //temp value and count how many quadruple code in the block, the states of if and while
    stack<string> id, op; //identifier and operator
    tv = code_in_block = ifValue = whileValue = 0;

    ifstream ifs;
    ifs.open("./input/main.c", ifstream::in);
    ofstream ofs;
    ofs.open("quadruples.txt", ofstream::out);

    while(getline(ifs, str))
    {
	isBreak = isNot = isMinus = isReturn = false;
	while(!id.empty() || !op.empty())
	    id.pop(), op.pop(); //empty the stacks

	if(ifValue == 0 && str.find("if") != string::npos) ifValue = 1;
	if(whileValue == 0 && str.find("while") != string::npos) whileValue = 1;

	stringstream ss (str);
	while(getline(ss, str, ' '))
	{
	    if(ss.fail()) break;
	    string::iterator it = str.begin();
	    while(*it == '\t')
		str.erase(0, 1);
	    if(str == "") continue;

	    if(str == "else") ifValue = 2; // go into another if block

	    switch(lexer(l_map, str))
	    {
		case 'K':
		    if(str == "int" || str == "char")
			isBreak = true;
		    else if(l_map.find(str)->second == "Operator")
		    {
			op.push(str);
			if(str == "!") isNot = true;
		    }
		    else if(str == "{")
		    {
			code_in_block = 0; 
			if(ifValue > 0) ifValue++; //into block of if 
			if(whileValue > 0) whileValue++; //into block of while
		    }
		    else if(str == ")" || str == "}" || str == "]" || str == ";")
			resultOutput(str, isNot, isReturn, tv, code_in_block, ifValue, whileValue, id, op, QT);
		    else if(str == "return")
			isReturn = true;
		    break;
		case 'I':
		case 'D':
		    {
			id.push(str);
			if(isNot) resultOutput("!", isNot, isReturn, tv, code_in_block, ifValue, whileValue, id, op, QT);
		    }
		    break;
	    }
	    if(isBreak) break;
	}
    }
    int count = 0;
    quadRuple Q;
    ofs<<setw(4)<<"line"<<setw(8)<<"op"<<setw(8)<<"arg1"<<setw(8)<<"arg2"<<setw(8)<<"result"<<endl;
    for(vector<quadRuple>::iterator it = QT.begin(); it != QT.end(); ++it)
    {
	Q = *it;
	ofs<<setw(4)<<count++<<setw(8)<<Q.op<<setw(8)<<Q.arg1<<setw(8)<<Q.arg2<<setw(8)<<Q.result<<endl;
    }
    ifs.close();
    ofs.close();
}

void resultOutput(string str, bool &isNot, bool &isReturn, int &tv, int &code_in_block, int &ifValue, int &whileValue, stack<string> &id, stack<string> &op, vector<quadRuple> &QT)
{
    string arg1, arg2;
    if(str == ";") //if the str is ";"
    {
	while(!id.empty()) //clean the identifier stack
	{
	    if(op.empty())
		break;
	    else if(op.top() != "=")
	    {
		arg2 = id.top(), id.pop();
		arg1 = id.top(), id.pop();
		QT.push_back(quadRupleMaker(op.top(), arg1, arg2, "t" + int2str(++tv)));
		op.pop();
		if(!id.empty()) id.push(QT[QT.size()-1].result);
	    }
	    else
	    {
		arg1 = id.top(), id.pop();
		QT.push_back(quadRupleMaker(op.top(), arg1, "", id.top()));
		op.pop(), id.pop();
	    }
	    code_in_block++;
	}
	if(isReturn) //special condition, return
	{
	    if(!id.empty())
	    {
		QT.push_back(quadRupleMaker("ret", id.top(), "", "t" + int2str(++tv)));
		id.pop();
	    }
	    else
		QT.push_back(quadRupleMaker("ret", QT[QT.size()-1].result, "", "t" + int2str(++tv)));
	    isReturn = false;
	}
    }
    else if(str == ")")
    {
	if(ifValue || whileValue) //this may be showed in the if statement and while loop
	{
	    if(op.empty())
	    {
		QT.push_back(quadRupleMaker("=", id.top(), "", "t" + int2str(++tv)));
		id.pop();
	    }
	    else
	    {
		arg2 = id.top(), id.pop();
		arg1 = id.top(), id.pop();
		QT.push_back(quadRupleMaker(op.top(), arg1, arg2, "t" + int2str(++tv)));
		op.pop();
	    }
	}
    }
    else if(str == "}")
    {
	//discuss the different condition if if statement and while loop
	if(ifValue == 2)
	    QT.insert(QT.end() - code_in_block, quadRupleMaker("jfalse", int2str(QT.size()+2), QT[QT.size()-code_in_block-1].result, ""));
	else if(ifValue == 3)
	    QT.insert(QT.end() - code_in_block, quadRupleMaker("jmp", int2str(QT.size()+1), QT[QT.size()-code_in_block-1].result, ""));
	else if(whileValue == 2)
	{
	    QT.insert(QT.end() - code_in_block, quadRupleMaker("jfalse", int2str(QT.size()+2), QT[QT.size()-code_in_block-1].result, ""));
	    QT.push_back(quadRupleMaker("jmp", int2str(QT.size()-code_in_block-2), "", ""));
	}
	ifValue = whileValue = 0;
    }
    else if(str == "]") //array
    {
	arg2 = id.top(), id.pop();
	arg1 = id.top(), id.pop();
	QT.push_back(quadRupleMaker("[]=", arg1, arg2, "t" + int2str(++tv)));
	id.push(QT[QT.size()-1].result);
	code_in_block++;
    }
    else if(str == "!") //not
    {
	QT.push_back(quadRupleMaker(op.top(), id.top(), "", "t" + int2str(++tv)));
	id.pop(), op.pop();
	id.push(QT[QT.size()-1].result);
	code_in_block++;
	isNot = false;
    }
    /*
       else if(str == "-")
       {
       Q.arg1 = id.top();
       id.pop();
       Q.op = op.top();
       op.pop();
       Q.arg2 = "";
       Q.result = "t" + int2str(++tv);
       id.push(Q.result);
       QT.push_back(Q);
       code_in_block++;
       }
     */
}

//integer to string
string int2str(int i)
{
    char s[10];
    sprintf(s, "%d", i);
    string ss(s);
    return ss;
}

//make a quadRuple variable
quadRuple quadRupleMaker(string a, string b, string c, string d)
{
    quadRuple Q;
    Q.op = a, Q.arg1 = b, Q.arg2 = c, Q.result = d;
    return Q;
}
