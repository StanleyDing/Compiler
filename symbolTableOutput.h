using namespace std;

struct symbolUnit
{
    string symbol, token, type;
    int scope;
}A;

void symbolTableOutput()
{
    int latest_scope = 0, used_scope = 0;
    bool isComment;
    string str, now_type;
    map<int, int> scope_map;
    stack<int> scope_stk;
    vector<symbolUnit> ST; //symbol table
    ifstream ifs;
    ifs.open("main.c", ifstream::in);
    ofstream ofs;
    ofs.open("symbol.txt", ofstream::out);
    scope_stk.push(0);
    scope_map.insert(pair<int, int>(0, 0));

    while(getline(ifs, str))
    {
	isComment = false;
	stringstream ss (str);
	while(getline(ss, str, ' '))
	{
	    if(ss.fail()) break;
	    for(string::iterator it = str.begin(); it != str.end(); ++it)
		if(*it == '\t')
		    str.erase(0, 1); //if the first symbol is tab

	    switch(lexer(str))
	    {
		case 'K':
		    {
			if(str == "int" || str == "char")
			    now_type = str;
			else if(str == "{" || str == "(")
			{
			    scope_stk.push(++latest_scope);
			}
			else if(str == "}" || str == ")")
			{
			    scope_stk.pop();
			}
			break;
		    }
		case 'I':
		    {
			vector<symbolUnit>::iterator it;
			for(it = ST.begin(); it != ST.end(); ++it)
			{
			    A = *it;
			    if(A.symbol == str)
				break;
			}
			if(it == ST.end())
			{
			    A.symbol = str;
			    A.token = "id";
			    A.type = now_type;
			    A.scope = scope_stk.top();
			    ST.push_back(A);
			    map<int, int>::iterator itr;
			    itr = scope_map.find(scope_stk.top());
			    if(itr == scope_map.end())
				used_scope++;
			    scope_map.insert(pair<int, int>(scope_stk.top(), used_scope));
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
	    if(isComment)
		break;
	}
    }
    for(vector<symbolUnit>::iterator it = ST.begin(); it != ST.end(); ++it)
    {
	A = *it;
	cout<<A.symbol<<" "<<A.token<<" "<<A.type<<" "<<" "<<scope_map.find(A.scope)->second<<endl;
    }
    ifs.close();
    ofs.close();
}

