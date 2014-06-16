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
struct Product_List *table_get_pl(struct LLTable *table, int lhs_id, int sym_id);

#endif
