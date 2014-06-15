#include <iomanip>
using namespace std;

#ifndef QUADRUPLESOUTPUT_H
#define QUADRUPLESOUTPUT_H
struct quadRuple
{
    string op, arg1, arg2, result;
}Q;
bool isNot, isMinus;
int tv = 0, lv = 0, code_in_block = 0; //temp value and label value
int ifValue = 0, whileValue = 0;
stack<string> id, op; //identifier and operator
vector<quadRuple> QT; //quadruples table
void resultOutput(string );
void int2str(int , char *);
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
	    if(str == "else")
		ifValue = 2;

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
			if(ifValue > 0) ifValue++;
			if(whileValue > 0) whileValue++;
		    }
		    else if(str == ")" || str == "}" || str == "]" || str == ";")
			resultOutput(str);
		    break;
		case 'I':
		case 'D':
		    {
			id.push(str);
			if(isNot)
			    resultOutput("!");
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
	cout<<setw(2)<<count++<<" "<<setw(8)<<Q.op<<setw(8)<<Q.arg1<<setw(8)<<Q.arg2<<setw(8)<<Q.result<<endl;
    }
    ifs.close();
    ofs.close();
}

void resultOutput(string str)
{
    vector<quadRuple>::iterator it;
    quadRuple Q;
    char s[10];
    if(str == ";")
    {
	while(!id.empty())
	{
	    if(op.top() != "=")
	    {
		Q.arg2 = id.top();
		id.pop();
		Q.arg1 = id.top();
		id.pop();
		Q.op = op.top();
		op.pop();
		int2str(++tv, s);
		string ss(s);
		Q.result = "t" + ss;
		if(!id.empty())
		    id.push(Q.result);
	    }
	    else
	    {
		Q.arg1 = id.top();
		id.pop();
		Q.op = "=";
		Q.result = id.top();
		id.pop();
		Q.arg2 = "";
		op.pop();
	    }

	    QT.push_back(Q);
	    code_in_block++;
	}
    }
    else if(str == ")")
    {
	if(ifValue || whileValue)
	{
	    Q.arg2 = id.top();
	    id.pop();
	    Q.arg1 = id.top();
	    id.pop();
	    Q.op = op.top();
	    op.pop();

	    int2str(++tv, s);
	    string ss(s);
	    Q.result = "t" + ss;
	    QT.push_back(Q);
	}
    }
    else if(str == "}")
    {
	if(ifValue == 2)
	{
	    Q.op = "jfalse";
	    int2str(QT.size()+2, s);
	    string ss(s);
	    Q.arg1 = ss;
	    Q.arg2 = QT[QT.size()-code_in_block-1].result;
	    Q.result = "";
	    it = QT.end();
	    QT.insert(it - code_in_block, Q);
	}
	else if(ifValue == 3)
	{
	    Q.op = "jmp";
	    int2str(QT.size()+1, s);
	    string ss(s);
	    Q.arg1 = ss;
	    Q.arg2 = QT[QT.size()-code_in_block-1].result;
	    Q.result = "";
	    it = QT.end();
	    QT.insert(it - code_in_block, Q);
	}

	if(whileValue == 2)
	{
	    Q.op = "jfalse";
	    int2str(QT.size()+2, s);
	    string ss(s);
	    Q.arg1 = ss;
	    Q.arg2 = QT[QT.size()-code_in_block-1].result;
	    Q.result = "";
	    it = QT.end();
	    QT.insert(it - code_in_block, Q);
	    Q.op = "jmp";
	    int2str(QT.size()-code_in_block-2, s);
	    string sss(s);
	    Q.arg1 = sss;
	    Q.arg2 = Q.result = "";
	    QT.push_back(Q);
	}

	ifValue = whileValue = 0;
    }
    else if(str == "]")
    {
	Q.arg2 = id.top();
	id.pop();
	Q.arg1 = id.top();
	id.pop();
	Q.op = "[]=";
	int2str(++tv, s);
	string ss(s);
	Q.result = "t" + ss;
	id.push(Q.result);
	QT.push_back(Q);
	code_in_block++;
    }
    else if(str == "!")
    {
	Q.arg1 = id.top();
	id.pop();
	Q.op = op.top();
	op.pop();
	Q.arg2 = "";
	int2str(++tv, s);
	string ss(s);
	Q.result = "t" + ss;
	id.push(Q.result);
	QT.push_back(Q);
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
	int2str(++tv, s);
	string ss(s);
	Q.result = "t" + ss;
	id.push(Q.result);
	QT.push_back(Q);
	code_in_block++;
    }
}

void int2str(int i, char *s)
{
    sprintf(s, "%d", i);
}
