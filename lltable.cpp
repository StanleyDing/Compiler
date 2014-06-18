#include "lltable.h"
#include <cstdlib>
#include <cstring>
#include <set>

void add_entry(struct LLTable *table, int lhs_id, int sym_id,
               struct Product_List *pl)
{
    struct LLTable_Entry *entry, *it;

    // check if the entry already exist
    for(it = table->entry; it; it = it->next)
        if(it->lhs_id == lhs_id && it->sym_id == sym_id && it->pl == pl)
            return;

    entry = (LLTable_Entry*) malloc(sizeof(LLTable_Entry));
    entry->lhs_id = lhs_id;
    entry->sym_id = sym_id;
    entry->pl = pl;
    entry->next = table->entry;

    table->entry = entry;
}

void print_table(struct Grammar *grammar, struct LLTable *table)
{
    FILE *fp = fopen("LLtable.txt", "w");
    Symbol **symbol_table;
    struct LLTable_Entry *entry;
    struct Product *product;

    symbol_table = grammar->symbol_table;
    entry = table->entry;

    while(entry){
        product = entry->pl->product;
        fprintf(fp, "%20s ", symbol_table[entry->lhs_id]->name);
        fprintf(fp, "%20s       ", symbol_table[entry->sym_id]->name);
        while(product){
            fprintf(fp, "%s ", symbol_table[product->id]->name);
            product = product->next;
        }
        entry = entry->next;
        fprintf(fp, "\n");
    }
    fclose(fp);
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

    // iterate the rules
    while(rule){
        pl = rule->pl;
        // iterate the product list
        while(pl){
            product = pl->product;
            // iterate the products
            while(product){
                sym = symbol_table[product->id];
                if(!strcmp(sym->name, "epsilon")){
                    product = product->next;
                    break;
                }
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

// get the product list by LHS symbol id and the symbol met
struct Product_List *table_get_pl(struct LLTable *table, int lhs_id, int sym_id)
{
    struct LLTable_Entry *entry;

    entry = table->entry;
    while(entry){
        if(entry->lhs_id == lhs_id && entry->sym_id == sym_id)
            return entry->pl;
        entry = entry->next;
    }
    return NULL;
}
