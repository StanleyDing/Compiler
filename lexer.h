using namespace std;

#ifndef NEW
#define NEW
map<string, string> mymap;
char lexer(string );
bool isKOS(string );
bool isDigit(string );
bool isInentifier(string );
bool isChar(string );
#endif

char lexer(string str)
{
    mymap.insert(pair<string, string>("int", "Keyword"));
    mymap.insert(pair<string, string>("char", "Keyword"));
    mymap.insert(pair<string, string>("return", "Keyword"));
    mymap.insert(pair<string, string>("if", "Keyword"));
    mymap.insert(pair<string, string>("else", "Keyword"));
    mymap.insert(pair<string, string>("while", "Keyword"));
    mymap.insert(pair<string, string>("break", "Keyword"));

    mymap.insert(pair<string, string>("=", "Operator"));
    mymap.insert(pair<string, string>("!", "Operator"));
    mymap.insert(pair<string, string>("+", "Operator"));
    mymap.insert(pair<string, string>("-", "Operator"));
    mymap.insert(pair<string, string>("*", "Operator"));
    mymap.insert(pair<string, string>("/", "Operator"));
    mymap.insert(pair<string, string>("==", "Operator"));
    mymap.insert(pair<string, string>("!=", "Operator"));
    mymap.insert(pair<string, string>("<", "Operator"));
    mymap.insert(pair<string, string>(">", "Operator"));
    mymap.insert(pair<string, string>("<=", "Operator"));
    mymap.insert(pair<string, string>(">=", "Operator"));
    mymap.insert(pair<string, string>("&&", "Operator"));
    mymap.insert(pair<string, string>("||", "Operator"));

    mymap.insert(pair<string, string>("[", "Special"));
    mymap.insert(pair<string, string>("]", "Special"));
    mymap.insert(pair<string, string>("(", "Special"));
    mymap.insert(pair<string, string>(")", "Special"));
    mymap.insert(pair<string, string>("{", "Special"));
    mymap.insert(pair<string, string>("}", "Special"));
    mymap.insert(pair<string, string>(";", "Special"));
    mymap.insert(pair<string, string>(",", "Special"));

    if(isKOS(str))
	return 'K'; //keyword, operator, special symbol
    else if(isDigit(str))
	return 'D'; //digit
    else if(isInentifier(str))
	return 'I'; //identifier
    else if(isChar(str))
	return 'C'; //char
    else if(str == "//")
	return 'M'; //comment
    else 
	return 'E'; //error
}

bool isKOS(string str)
{
    //keyword, operastor, special
    map<string, string>::iterator it = mymap.find(str);
    if(it != mymap.end())
	return true;
    else
	return false;
}

bool isDigit(string str)
{
    //digit
    for(string::iterator it = str.begin(); it != str.end(); ++it)
	if(!isdigit(*it))
	    return false;
    return true;
}

bool isInentifier(string str)
{
    //identifier
    for(string::iterator it = str.begin(); it != str.end(); ++it)
	if(it == str.begin())
	{
	    if(!isalpha(*it) && *it != '_')
		return false;
	}
	else
	    if(!isalnum(*it) && *it != '_')
		return false;
    return true;
}

bool isChar(string str)
{
    //char
    if(str[0] == '\'' && str[2] == '\'')
    {
	if(isgraph(str[1]))
	    return true;
    }
    else if(str == "\'\\t\'" || str == "\'\\n\'" || str == "\' \'")
	return true;
    return false;
}
