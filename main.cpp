#include "set.h"
#include "grammar.h"
#include "lltable.h"

int main()
{
    struct Grammar grammar;
    struct LLTable table;

    table.entry = NULL;

    parser_gen(&grammar, "./zip/perfect_grammar.txt");
    build_lltable(&grammar, &table);
    
    return 0;
}
