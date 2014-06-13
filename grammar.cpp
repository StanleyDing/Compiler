#include <cstring>
#include <cstdlib>
#include "grammar.h"
#include "hash.h"

Symbol *insert_symbol(Symbol_List *hash_table[], char *str, int sym_id)
{
    int hash_value;
    Symbol *new_sym;
    Symbol_List *tmp, *sym_list;

    if(sym_id == MAX_SYMBOL)
        return NULL;

    hash_value = hash(str) % MAX_HASH_BUCKET;

    tmp = hash_table[hash_value];

    // check if the symbol exist
    while(tmp){
        if(!strcmp(tmp->sym->name, str))
            return NULL;
        tmp = tmp->next;
    }

    new_sym = create_symbol(sym_id, str);

    sym_list = (Symbol_List*) malloc(sizeof(Symbol_List));
    sym_list->sym = new_sym;
    sym_list->next = hash_table[hash_value];
    hash_table[hash_value] = sym_list;

    return new_sym;
}

int symbol_id(Symbol_List *hash_table[], char *str)
{
    int hash_value;
    Symbol_List *tmp;

    hash_value = hash(str) % MAX_HASH_BUCKET;

    tmp = hash_table[hash_value];

    while(tmp){
        if(!strcmp(tmp->sym->name, str))
            return tmp->sym->id;
        tmp = tmp->next;
    }

    return -1;
}

Symbol *create_symbol(int id, char *str)
{
    Symbol *sym;

    sym = (Symbol*) malloc(sizeof(Symbol));

    sym->id = id;
    sym->name = (char*) malloc(strlen(str) + 1);
    strcpy(sym->name, str);
    sym->terminal = 1;
    sym->nullable = 0;

    return sym;
}

void delete_symbol(Symbol *sym)
{
    if(!sym) return;

    free(sym->name);
    free(sym);
}

void push_product(struct Product_List *pl, int sym_id)
{
    if(!pl) return;

    struct Product *p, *new_p;

    new_p = (struct Product*) malloc(sizeof(struct Product));
    new_p->id = sym_id;
    new_p->next = NULL;

    p = pl->product;
    while(p && p->next)
        p = p->next;
    if(!p)
        pl->product = new_p;
    else
        p->next = new_p;
}

struct Product_List *create_productlist()
{
    struct Product_List *pl;

    pl = (struct Product_List*) malloc(sizeof(struct Product_List));
    pl->product = NULL;
    pl->next = NULL;

    return pl;
}

void add_productlist(struct Rule *rule, struct Product_List *pl)
{
    if(!rule || !pl)
        return;

    pl->next = rule->pl;
    rule->pl = pl;
}

struct Rule *create_rule(int lhs_id)
{
    struct Rule *rule;

    rule  = (struct Rule*) malloc(sizeof(Rule));
    rule->lhs_id = lhs_id;
    rule->pl = NULL;
    rule->next = NULL;

    return rule;
}

struct Rule *find_rule(struct Grammar *grammar, int lhs_id)
{
    if(!grammar)
        return NULL;

    struct Rule *rule = grammar->rule;

    while(rule && rule->lhs_id != lhs_id)
        rule = rule->next;

    return rule;
}

void grammar_init(struct Grammar *grammar)
{
    if(!grammar)
        return;

    grammar->rule = NULL;
}

void grammar_addrule(struct Grammar *grammar, struct Rule *rule)
{
    if(!grammar || !rule)
        return;

    rule->next = grammar->rule;
    grammar->rule = rule;
}
