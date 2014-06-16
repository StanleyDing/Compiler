#include "symbolTableOutput.h"
#include "lexer.h"
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

void symbolTableOutput(std::map<string, string> &l_map)
{
    int latest_scope = 0, used_scope = -1;
    bool isComment, isError;
    string str, now_type;
    map<int, int> scope_map;
    stack<int> scope_stk;
    vector<symbolUnit> ST; //symbol table
    struct symbolUnit A;
    ifstream ifs;
    ifs.open("main.c", ifstream::in);
    ofstream ofs;
    ofs.open("symbol.txt", ofstream::out);
    scope_stk.push(0);

    while(getline(ifs, str))
    {
	isComment = isError = false;
	stringstream ss (str);

	if(str.find("int") == string::npos && str.find("char") == string::npos)
	    continue;

	while(getline(ss, str, ' '))
	{
	    if(ss.fail()) break;
	    string::iterator it = str.begin();
	    while(*it == '\t')
		str.erase(0, 1); //if the first symbol is tab
	    if(str == "") continue;

	    switch(lexer(l_map, str))
	    {
		case 'K':
		    {
			if(str == "int" || str == "char")
			    now_type = str;
			else if(str == "{" || str == "(")
			    scope_stk.push(++latest_scope);
			else if(str == "}" || str == ")")
			    scope_stk.pop();
			else if(str == "[")
			    ST[ST.size() -1].type += " arr";
			break;
		    }
		case 'I':
		    {
			vector<symbolUnit>::iterator it;
			for(it = ST.begin(); it != ST.end(); ++it)
			{
			    A = *it;
			    if(A.symbol == str && A.type == now_type && A.scope == scope_stk.top()) break;
			    else if(A.symbol == str && A.scope == scope_stk.top() && (A.type != now_type))
			    {
				isError = true;
				break;
			    }
			}
			if(it == ST.end() && !isError)
			{
			    A.symbol = str;
			    A.token = "id";
			    A.type = now_type;
			    A.scope = scope_stk.top();
			    ST.push_back(A);
			    map<int, int>::iterator itr;
			    itr = scope_map.find(scope_stk.top());
			    if(itr == scope_map.end())
				scope_map.insert(pair<int, int>(scope_stk.top(), ++used_scope));
			    else
				scope_map.insert(pair<int, int>(scope_stk.top(), itr->second));
			}
			break;
		    }
		case 'M':
		    isComment = true;
		    break;
		case 'D':
		case 'C':
		case 'E':
		    break;
	    }
	    if(isComment || isError) break;
	}
	if(isError) break;
    }
    if(isError)
	ofs<<"Error occurred."<<endl;
    else
    {
	ofs<<setw(8)<<"symbol"<<" "<<setw(8)<<"token"<<" "<<setw(8)<<"type"<<" "<<" "<<setw(8)<<"scope"<<endl;
	for(vector<symbolUnit>::iterator it = ST.begin(); it != ST.end(); ++it)
	{
	    A = *it;
	    ofs<<setw(8)<<A.symbol<<" "<<setw(8)<<A.token<<" "<<setw(8)<<A.type<<" "<<" "<<setw(8)<<scope_map.find(A.scope)->second<<endl;
	}
    }
    ifs.close();
    ofs.close();
}
