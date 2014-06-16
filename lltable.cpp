#include "lltable.h"
#include <cstdlib>
#include <set>

void add_entry(struct LLTable *table, int lhs_id, int sym_id,
               struct Product_List *pl)
{
    struct LLTable_Entry *entry;

    entry = (LLTable_Entry*) malloc(sizeof(LLTable_Entry));
    entry->lhs_id = lhs_id;
    entry->sym_id = sym_id;
    entry->pl = pl;
    entry->next = table->entry;

    table->entry = entry;
}

void print_table(struct Grammar *grammar, struct LLTable *table)
{
    Symbol **symbol_table;
    struct LLTable_Entry *entry;
    struct Product *product;

    symbol_table = grammar->symbol_table;
    entry = table->entry;

    while(entry){
        product = entry->pl->product;
        printf("%15s ", symbol_table[entry->lhs_id]->name);
        printf("%15s       ", symbol_table[entry->sym_id]->name);
        while(product){
            printf("%s ", symbol_table[product->id]->name);
            product = product->next;
        }
        entry = entry->next;
        printf("\n");
    }
}

void build_lltable(struct Grammar *grammar, struct LLTable *table)
{
    struct Rule *rule;
    struct Product_List *pl;
    struct Product *product;
    Symbol *sym;
    Symbol **symbol_table;
    std::set<int>::iterator it;

    rule = grammar->rule;
    symbol_table = grammar->symbol_table;

    while(rule){
        pl = rule->pl;
        while(pl){
            product = pl->product;
            while(product){
                sym = symbol_table[product->id];
                it = sym->first.begin();
                while(it != sym->first.end()){
                    add_entry(table, rule->lhs_id, *it, pl);
                    it++;
                }
                if(symbol_table[product->id]->nullable)
                    product = product->next;
                else
                    break;
            }
            if(!product){
                sym = symbol_table[rule->lhs_id];
                it = sym->follow.begin();
                while(it != sym->follow.end()){
                    add_entry(table, rule->lhs_id, *it, pl);
                    it++;
                }
            }
            pl = pl->next;
        }
        rule = rule->next;
    }
    print_table(grammar, table);
}
