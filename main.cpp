#include <map>
#include <cstdio>
#include "set.h"
#include "grammar.h"
#include "lltable.h"
#include "lexer.h"
#include "lexerOutput.h"
#include "symbolTableOutput.h"
#include "quadRuplesOutput.h"
#include "machineCodeOutput.h"
#include "parse_tree.h"

using namespace std;

int main()
{
    struct Grammar grammar;
    struct LLTable table;
    map<string, string> lexer_map;
    vector<symbolUnit> ST; //symbol table
    vector<quadRuple> QT;  //quadruples table
    table.entry = NULL;
    lexer_map_init(lexer_map);

    //parser_gen(&grammar, "./zip/perfect_grammar.txt");
    //build_lltable(&grammar, &table);
    
    lexicalOutput(lexer_map);
    symbolTableOutput(lexer_map, ST);
    quadRuplesOutput(lexer_map, QT);
    machineCodeOutput(ST, QT);
  
    parse_tree(&grammar, &table, lexer_map, "token.txt");
    return 0;
}
