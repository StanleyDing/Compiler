#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lexer.h"
#include "lexerOutput.h"

void lexicalOutput(std::map<string, string> &l_map)
{
    int line = 0;   //current line of code
    bool isComment = false; //if the input is comment or not
    string str;
    ifstream ifs;
    ifs.open("main.c", ifstream::in);
    ofstream ofs;
    ofs.open("token.txt", ofstream::out);

    while(getline(ifs, str)) //get a line of input
    {
	isComment = false;
	ofs<<"Line : "<<++line<<endl;
	stringstream ss (str); 
	while(getline(ss, str, ' ')) //using a space to cut the input
	{
	    if(ss.fail()) break;
	    string::iterator it = str.begin();
	    while(*it == '\t')
		str.erase(0, 1); //erase the tab
	    if(str == "") continue;

	    switch(lexer(l_map, str))
	    {
		case 'K': //keyword, operator, special words
		    if(l_map.find(str)->second == "Operator")
			ofs<<"\t"<<"  <Operator> : "<<str<<endl;
		    else if(l_map.find(str)->second == "Keyword")
			ofs<<"\t"<<"   <Keyword> : "<<str<<endl;
		    else if(l_map.find(str)->second == "Special")
			ofs<<"\t"<<"   <Special> : "<<str<<endl;
		    break;
		case 'D': //digit
		    ofs<<"\t"<<"    <Number> : "<<str<<endl;
		    break;
		case 'I': //identifier
		    ofs<<"\t"<<"<Identifier> : "<<str<<endl;
		    break;
		case 'C': //char
		    ofs<<"\t"<<"      <Char> : "<<str<<endl;
		    break;
		case 'M': //comment
		    isComment = true;
		    break;
		case 'E': //error
		    ofs<<"\t"<<"     <Error> : "<<str<<endl;
		    break;
	    }
	    if(isComment)
		break;
	}
    }
    ofs<<"\t"<<"   <Special> : $"<<endl;
    ifs.close();
    ofs.close();
}
