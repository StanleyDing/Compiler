#ifndef LLTABLE_H
#define LLTABLE_H

#include "grammar.h"

struct LLTable_Entry
{
    int lhs_id;
    int sym_id;
    struct Product_List *pl;
    LLTable_Entry *next;
};

struct LLTable
{
    struct LLTable_Entry *entry;
};

void print_table(struct Grammar *grammar, struct LLTable *table);
void build_lltable(struct Grammar *grammar, struct LLTable *table);
void build_lltable(struct Grammar *grammar, struct LLTable *table);
#endif
