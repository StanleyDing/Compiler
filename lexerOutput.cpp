#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lexer.h"
#include "lexerOutput.h"

void lexicalOutput(std::map<string, string> &l_map)
{
    int line = 0;   //line of code
    bool isComment = false;
    string str;
    ifstream ifs;
    ifs.open("main.c", ifstream::in);
    ofstream ofs;
    ofs.open("token.txt", ofstream::out);

    while(getline(ifs, str))
    {
	isComment = false;
	ofs<<"Line : "<<++line<<endl;
	stringstream ss (str);
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
		    if(l_map.find(str)->second == "Operator")
			ofs<<"\t"<<"  <Operator> : "<<str<<endl;
		    else if(l_map.find(str)->second == "Keyword")
			ofs<<"\t"<<"   <Keyword> : "<<str<<endl;
		    else if(l_map.find(str)->second == "Special")
			ofs<<"\t"<<"   <Special> : "<<str<<endl;
		    break;
		case 'D':
		    ofs<<"\t"<<"    <Number> : "<<str<<endl;
		    break;
		case 'I':
		    ofs<<"\t"<<"<Identifier> : "<<str<<endl;
		    break;
		case 'C':
		    ofs<<"\t"<<"      <Char> : "<<str<<endl;
		    break;
		case 'M':
		    isComment = true;
		    break;
		case 'E':
		    ofs<<"\t"<<"     <Error> : "<<str<<endl;
		    break;
	    }
	    if(isComment)
		break;
	}
    }
    ifs.close();
    ofs.close();
}
