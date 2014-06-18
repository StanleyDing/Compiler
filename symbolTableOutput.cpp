#include "symbolTableOutput.h"
#include "lexer.h"
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>

void symbolTableOutput(std::map<string, string> &l_map, vector<symbolUnit> &ST)
{
    int latest_scope = 0, used_scope = -1; //count the scope number which is used
    bool isComment, isError; 
    string str, now_type;
    map<int, int> scope_map; //map the latest scope to the used scope
    stack<int> scope_stk; //store the latest scope
    struct symbolUnit A;
    ifstream ifs;
    ifs.open("./input/main.c", ifstream::in);
    ofstream ofs;
    ofs.open("symbol.txt", ofstream::out);
    scope_stk.push(0);

    while(getline(ifs, str))
    {
	isComment = isError = false;
	stringstream ss (str);

	if(str.find("int") == string::npos && str.find("char") == string::npos)
	    continue; //if this line doesn't have int or char, it means that it doesn't have new identifier

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
			    now_type = str; //the type used now is stored in now_type
			else if(str == "{" || str == "(")
			    scope_stk.push(++latest_scope); //enter to a new block
			else if(str == "}" || str == ")") //exit from a block
			    scope_stk.pop();
			else if(str == "[")
			    ST[ST.size() -1].type += " arr"; //the type is array
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
				break; //if two identifiers have same name and at same scope, but have different types, then Error occues
			    }
			}
			if(it == ST.end() && !isError) //if we cannot find a symbol in symbol table, then add it to symbol table
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
		    if(ST[ST.size()-1].type.find("arr") != string::npos) //the usage of memory used in the array type
			ST[ST.size()-1].vol = atoi(str.c_str());
		    else 
			ST[ST.size()-1].vol = 1;
		case 'C':
		case 'E':
		    break;
	    }
	    if(isComment || isError) break;
	}
	if(isError) break;
    }
    if(isError) //redefine in the same scope
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
