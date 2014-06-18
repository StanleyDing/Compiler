#include "parse_tree.h"
#include "lltable.h"
#include "lexer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>

using namespace std;

void print_indent(FILE *fp, int n)
{
    n *= 2;
    for(int i = 0; i < n; i++)
        fprintf(fp, " ");
}

void parse_tree(struct Grammar *grammar, struct LLTable *table, 
                map<string, string> &l_map, char *tok_path)
{
    FILE *fp, *tree_txt;
    int sym_id, consume, more;
    char s1[50], s2[50], tok_type;
    string str;
    struct PLstack *top, *new_stk;
    Symbol **symbol_table;
    Symbol *sym, *top_sym;
    struct Product_List *pl;

    fp = fopen(tok_path, "r");
    tree_txt = fopen("tree.txt", "w");

    sym_id = symbol_id(grammar->hash_table, "S");
    if(sym_id == -1){
        printf("Can't find start symbol: S\n");
        fclose(fp);
        return;
    }

    symbol_table = grammar->symbol_table;
    top = create_plstack(0, find_rule(grammar, sym_id)->pl);

    while(fscanf(fp, "%s : %s", s1, s2) != EOF){
        if(!strcmp("Line", s1))
            continue;

        more = 0;
        // the id of the symbol read
        sym_id = symbol_id(grammar->hash_table, s2);
        if(sym_id == -1){
            str = s2;
            tok_type = lexer(l_map, str);
            switch(tok_type){
                case 'D':
                    {
                        more = 1;
                        strcpy(s1, s2);
                        strcpy(s2, "num");
                        sym_id = symbol_id(grammar->hash_table, s2);
                    } break;
                case 'I':
                    {
                        more = 1;;
                        strcpy(s1, s2);
                        strcpy(s2, "id");
                        sym_id = symbol_id(grammar->hash_table, s2);
                    } break;
                default:
                    printf("Illegal symbol: %s\n", s2);
                    fclose(fp);
                    return;
            }
        }
        consume = 0;

        while(!consume){
            top_sym = symbol_table[top->p->id];
            // the top of stack is a terminal
            if(top_sym->terminal){
                if(!strcmp(s2, top_sym->name)){
                    print_indent(tree_txt, top->depth);
                    fprintf(tree_txt, "%d  %s\n", top->depth, top_sym->name);
                    if(more){
                        print_indent(tree_txt, top->depth + 1);
                        fprintf(tree_txt, "%d  %s\n", top->depth + 1, s1);
                    }
                    top->p = top->p->next;
                    consume = 1;
                }
                else{
                    printf("Error: top of stack doesn't match input.\n");
                    printf("Read: %s, find: %s\n", s2, top_sym->name);
                    fclose(fp);
                    return;
                }
            }
            // the top of stack is a nonterminal
            else{
                print_indent(tree_txt, top->depth);
                fprintf(tree_txt, "%d  %s\n", top->depth, top_sym->name);
                // shift for the nonterminal
                top->p = top->p->next;

                // consult the LLTable
                pl = table_get_pl(table, top_sym->id, sym_id);
                if(pl){
                    new_stk = create_plstack(top->depth + 1, pl);
                    top = push_pl(top, new_stk);
                }
                else{
                    printf("Syntax error.\n");
                    while(top)
                        pop_pl(&top);
                    return;
                }
            }
            while(1){
                if(!top)
                    return;
                // if the rule is not finished
                if(top->p){
                    top_sym = symbol_table[top->p->id];
                    // and there's no epsilon
                    if(strcmp(top_sym->name, "epsilon"))
                        break;
                    else
                        pop_pl(&top);
                }
                else
                    pop_pl(&top);
            }
        }
    }

    fclose(fp);
    fclose(tree_txt);
}

struct PLstack *push_pl(struct PLstack *top, struct PLstack *stk)
{
    stk->next = top;
    return stk;
}

void *pop_pl(struct PLstack **top)
{
    struct PLstack *tmp = (*top)->next;
    free(*top);
    *top = tmp;
}

struct PLstack *create_plstack(int depth, struct Product_List *pl)
{
    PLstack *plstack = (struct PLstack*) malloc(sizeof(struct PLstack));

    if(!plstack)
        return NULL;

    plstack->depth = depth;
    plstack->pl = pl;
    plstack->p = pl->product;
    plstack->next = NULL;

    return plstack;
}
