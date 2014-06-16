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
    sym->first.clear();
    sym->follow.clear();

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
    memset(grammar->symbol_table, 0, sizeof(grammar->symbol_table));
    memset(grammar->hash_table, 0, sizeof(grammar->hash_table));
}

void grammar_addrule(struct Grammar *grammar, struct Rule *rule)
{
    if(!grammar || !rule)
        return;

    rule->next = grammar->rule;
    grammar->rule = rule;
}

int set_union(std::set<int> &S1, std::set<int> &S2)
{
    std::set<int>::iterator it;
    int update = 0;

    for(it = S2.begin(); it != S2.end(); it++){
        if(S1.count(*it) == 0){
            update = 1;
            S1.insert(*it);
        }
    }
    return update;
}

int string_nullable(Symbol *symbol_table[], struct Product *product)
{
    while(product){
        if(!symbol_table[product->id]->nullable)
            return 0;
        product = product->next;
    }
    return 1;
}

void nullable(struct Grammar *grammar, Symbol *symbol_table[])
{
    int change = 1;
    struct Rule *rule = grammar->rule;
    struct Product_List *pl;
    struct Product *p;
    Symbol *symbol;

    while(change){
        change = 0;
        while(rule){
            symbol = symbol_table[rule->lhs_id];
            pl = rule->pl;
            while(!symbol->nullable && pl){
                p = pl->product;
                while(p){
                    if(symbol_table[p->id]->nullable)
                        p = p->next;
                    else break;
                }
                if(p == NULL){
                    symbol->nullable = 1;
                    change = 1;
                    break;
                }
                pl = pl->next;
            }
            rule = rule->next;
        }
    }
}

void first(struct Grammar *grammar, Symbol *symbol_table[], int sym_id)
{
    Symbol *sym;
    Symbol_List *sl;
    struct Rule *rule;
    struct Product_List *pl;
    struct Product *product;

    sym = symbol_table[sym_id];

    if(sym->terminal)
        sym->first.insert(sym->id);
    else{
        rule = find_rule(grammar, sym_id);
        if(rule == NULL)
            return;
        pl = rule->pl;
        while(pl){
            product = pl->product;
            while(product){
                first(grammar, symbol_table, product->id);
                set_union(sym->first, symbol_table[product->id]->first);
                if(!symbol_table[product->id]->nullable)
                    break;
                product = product->next;
            }
            pl = pl->next;
        }
    }
}

void follow(struct Grammar *grammar, Symbol *symbol_table[])
{
    struct Rule *rule;
    struct Product_List *pl;
    struct Product *product, *follow;
    Symbol *sym, *sym_follow;
    int update;

    rule = grammar->rule;
    while(rule){
        pl = rule->pl;
        while(pl){
            product = pl->product;
            while(product){
                sym = symbol_table[product->id];
                follow = product->next;
                if(!sym->terminal && follow)
                    set_union(sym->follow, symbol_table[follow->id]->first);
                product = product->next;
            }
            pl = pl->next;
        }
        rule = rule->next;
    }

    while(1){
        update = 0;
        rule = grammar->rule;
        while(rule){
            pl = rule->pl;
            while(pl){
                product = pl->product;
                while(product){
                    sym = symbol_table[product->id];
                    if(!sym->terminal &&
                            string_nullable(symbol_table, product->next))
                        if(set_union(sym->follow, symbol_table[rule->lhs_id]->follow))
                            update = 1;
                    product = product->next;
                }
                pl = pl->next;
            }
            rule = rule->next;
        }
        if(!update)
            break;
    }
}
