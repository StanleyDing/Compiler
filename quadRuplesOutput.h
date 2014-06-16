#include <iomanip>
using namespace std;

#ifndef QUADRUPLESOUTPUT_H
#define QUADRUPLESOUTPUT_H
struct quadRuple
{
    string op, arg1, arg2, result;
}Q;
bool isNot, isMinus;
int tv = 0, code_in_block = 0; //temp value and count how many quadruple code in the block
int ifValue = 0, whileValue = 0;
stack<string> id, op; //identifier and operator
vector<quadRuple> QT; //quadruples table
void resultOutput(string );
string int2str(int *);
quadRuple quadRupleMaker(string, string, string, string);

#endif

void quadRuplesOutput()
{
    bool isBreak;
    string str, label;
    ifstream ifs;
    ifs.open("main.c", ifstream::in);
    ofstream ofs;
    ofs.open("quadruples.txt", ofstream::out);

    while(getline(ifs, str))
    {
	isBreak = isNot = isMinus = false;
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

	    switch(lexer(str))
	    {
		case 'K':
		    if(str == "int" || str == "char")
			isBreak = true;
		    else if(mymap.find(str)->second == "Operator")
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
			resultOutput(str);
		    break;
		case 'I':
		case 'D':
		    {
			id.push(str);
			if(isNot) resultOutput("!");
		    }
		    break;
	    }
	    if(isBreak) break;
	}
    }
    int count = 0;
    for(vector<quadRuple>::iterator it = QT.begin(); it != QT.end(); ++it)
    {
	Q = *it;
	ofs<<setw(2)<<count++<<" "<<setw(8)<<Q.op<<setw(8)<<Q.arg1<<setw(8)<<Q.arg2<<setw(8)<<Q.result<<endl;
    }
    ifs.close();
    ofs.close();
}

void resultOutput(string str)
{
    vector<quadRuple>::iterator it;
    string arg1, arg2, result;
    quadRuple Q;
    if(str == ";")
    {
	while(!id.empty())
	{
	    if(op.top() != "=")
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
    }
    else if(str == ")")
    {
	if(ifValue || whileValue)
	{
	    arg2 = id.top(), id.pop();
	    arg1 = id.top(), id.pop();
	    QT.push_back(quadRupleMaker(op.top(), arg1, arg2, "t" + int2str(++tv)));
	    op.pop();
	}
    }
    else if(str == "}")
    {
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
    else if(str == "]")
    {
	arg2 = id.top(), id.pop();
	arg1 = id.top(), id.pop();
	QT.push_back(quadRupleMaker("[]=", arg1, arg2, "t" + int2str(++tv)));
	id.push(QT[QT.size()-1].result);
	code_in_block++;
    }
    else if(str == "!")
    {
	QT.push_back(quadRupleMaker(op.top(), id.top(), "", "t" + int2str(++tv)));
	id.pop(), op.pop();
	id.push(QT[QT.size()-1].result);
	code_in_block++;
	isNot = false;
    }
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
}

string int2str(int i)
{
    char s[10];
    sprintf(s, "%d", i);
    string ss(s);
    return ss;
}

quadRuple quadRupleMaker(string a, string b, string c, string d)
{
    quadRuple Q;
    Q.op = a, Q.arg1 = b, Q.arg2 = c, Q.result = d;
    return Q;
}
