#include <map>
#include "set.h"
#include "grammar.h"
#include "lltable.h"
#include "lexer.h"
#include "lexerOutput.h"
#include "symbolTableOutput.h"
#include "quadRuplesOutput.h"

using namespace std;

int main()
{
    struct Grammar grammar;
    struct LLTable table;
    map<string, string> lexer_map;
    table.entry = NULL;
    lexer_map_init(lexer_map);

    parser_gen(&grammar, "./zip/perfect_grammar.txt");
    build_lltable(&grammar, &table);
    
    lexicalOutput(lexer_map);
    symbolTableOutput(lexer_map);
    quadRuplesOutput(lexer_map);
  
    return 0;
}
