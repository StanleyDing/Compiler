#include "lexer.h"

void lexer_map_init(map<string, string> &l_map)
{
    l_map.insert(pair<string, string>("int", "Keyword"));
    l_map.insert(pair<string, string>("char", "Keyword"));
    l_map.insert(pair<string, string>("return", "Keyword"));
    l_map.insert(pair<string, string>("if", "Keyword"));
    l_map.insert(pair<string, string>("else", "Keyword"));
    l_map.insert(pair<string, string>("while", "Keyword"));
    l_map.insert(pair<string, string>("break", "Keyword"));

    l_map.insert(pair<string, string>("=", "Operator"));
    l_map.insert(pair<string, string>("!", "Operator"));
    l_map.insert(pair<string, string>("+", "Operator"));
    l_map.insert(pair<string, string>("-", "Operator"));
    l_map.insert(pair<string, string>("*", "Operator"));
    l_map.insert(pair<string, string>("/", "Operator"));
    l_map.insert(pair<string, string>("==", "Operator"));
    l_map.insert(pair<string, string>("!=", "Operator"));
    l_map.insert(pair<string, string>("<", "Operator"));
    l_map.insert(pair<string, string>(">", "Operator"));
    l_map.insert(pair<string, string>("<=", "Operator"));
    l_map.insert(pair<string, string>(">=", "Operator"));
    l_map.insert(pair<string, string>("&&", "Operator"));
    l_map.insert(pair<string, string>("||", "Operator"));

    l_map.insert(pair<string, string>("[", "Special"));
    l_map.insert(pair<string, string>("]", "Special"));
    l_map.insert(pair<string, string>("(", "Special"));
    l_map.insert(pair<string, string>(")", "Special"));
    l_map.insert(pair<string, string>("{", "Special"));
    l_map.insert(pair<string, string>("}", "Special"));
    l_map.insert(pair<string, string>(";", "Special"));
    l_map.insert(pair<string, string>(",", "Special"));
}

char lexer(map<string, string> &l_map, string str)
{
    if(isKOS(l_map, str))
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

bool isKOS(map<string, string> &l_map, string str)
{
    //keyword, operastor, special
    map<string, string>::iterator it = l_map.find(str);
    if(it != l_map.end())
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
