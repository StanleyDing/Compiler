#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <cstdio>
#include <set>
#define MAX_SYMBOL 200

typedef struct Symbol
{
    int id;
    char *name;
    int terminal;
    int nullable;
    std::set<int> first;
    std::set<int> follow;
} Symbol;

typedef struct Symbol_List
{
    struct Symbol *sym;
    struct Symbol_List *next;
} Symbol_List;

struct Product
{
    int id;
    struct Product *next;
};

struct Product_List
{
    struct Product *product;
    struct Product_List *next;
};

struct Rule
{
    int lhs_id;
    struct Product_List *pl;
    struct Rule *next;
};

struct Grammar
{
    struct Rule *rule;
    Symbol *symbol_table[MAX_SYMBOL];
};

Symbol *insert_symbol(Symbol_List *hash_table[], char *str, int sym_id);

int symbol_id(Symbol_List *hash_table[], char *str);

Symbol *create_symbol(int id, char *str);

void delete_symbol(Symbol *sym);

void push_product(struct Product_List *pl, int sym_id);

struct Product_List *create_productlist();

void add_productlist(struct Rule *rule, struct Product_List *pl);

struct Rule *create_rule(int lhs_id);

struct Rule *find_rule(struct Grammar *grammar, int lhs_id);

void grammar_init(struct Grammar *grammar);

void grammar_addrule(struct Grammar *grammar, struct Rule *rule);

int set_union(std::set<int> &S1, std::set<int> &S2);

void nullable(struct Grammar *grammar, Symbol *symbol_table[]);

void first(struct Grammar *grammar, Symbol *symbol_table[], int sym_id);

void follow(struct Grammar *grammar, Symbol *symbol_table[]);

#endif
