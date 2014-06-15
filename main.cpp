#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <ctype.h>
#include <iomanip>
#include "set.h"
#include "grammar.h"
#include "lltable.h"
#include "lexerOutput.h"
#include "symbolTableOutput.h"

using namespace std;

int main()
{
    struct Grammar grammar;
    struct LLTable table;

    table.entry = NULL;

    parser_gen(&grammar, "./zip/perfect_grammar.txt");
    build_lltable(&grammar, &table);
    
    lexicalOutput();
    symbolTableOutput();
  
    return 0;
}
