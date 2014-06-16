#ifndef PARSETREE_H
#define PARSETREE_H

#include "grammar.h"
#include <map>
#include <string>

using namespace std;

struct PLstack
{
    int depth;
    struct Product_List *pl;
    struct Product *p;
    struct PLstack *next;
};

void parse_tree(struct Grammar *grammar, struct LLTable *table,
                map<string, string> &l_map, char *tok_path);
struct PLstack *push_pl(struct PLstack *top, struct PLstack *stk);
void *pop_pl(struct PLstack **top);
struct PLstack *create_plstack(int depth, struct Product_List *pl);

#endif
