#include <cstdio>
#include <cstring>
#include "set.h"
#include "grammar.h"
#include "hash.h"

#define MAX_STR_LEN 100
int sym_num = 0;

void read_grammar(FILE *fp,
                  struct Grammar *grammar, 
                  Symbol_List *hash_table[],
                  Symbol *symbol_table[])
{
    int lhs_id = -1;
    int sym_id;
    unsigned int hash_value;
    char buf[MAX_STR_LEN+1], str[MAX_STR_LEN+1];
    Symbol *sym;
    struct Rule *rule;

    while(fgets(buf, MAX_STR_LEN + 1, fp)){
        switch(buf[0]){
            case ' ':   // RHS
            case '\t':  // RHS
                {
                    printf("\tRHS: ");
                    if(lhs_id == -1){
                        printf("No LHS found\n");
                        return;
                    }

                    struct Product *product;
                    struct Product_List *pl = NULL;
                    char *tok;

                    tok = strtok(buf, " \t\n\r");
                    if(tok){
                        pl = create_productlist();
                        if(!strcmp("epsilon", tok))
                            symbol_table[lhs_id]->nullable = 1;
                    }
                    while(tok){
                        printf("%s ", tok);
                        sym_id = symbol_id(hash_table, tok);
                        if(sym_id == -1){
                            sym = insert_symbol(hash_table, tok, sym_num);
                            sym_id = sym->id;
                            symbol_table[sym_num++] = sym;
                        }
                        push_product(pl, sym_id);
                        tok = strtok(NULL, " \t\n\r");
                    }

                    add_productlist(rule, pl);
                    printf("\n");
                } break;
            case '\n':  // new line
                break;
            default:    // LHS (nonterminal)
                {
                    sscanf(buf, "%s", str);

                    lhs_id = symbol_id(hash_table, str);
                    if(lhs_id == -1){
                        sym = insert_symbol(hash_table, str, sym_num);
                        lhs_id = sym_num;
                        symbol_table[sym_num++] = sym;
                    }

                    rule = find_rule(grammar, lhs_id);
                    // a new nonterminal
                    if(rule == NULL){
                        printf("new LHS: %s\n", str);
                        rule = create_rule(lhs_id);
                        grammar_addrule(grammar, rule);
                    }
                    // nonterminal already existed
                    else{
                        printf("old LHS: %s\n", str);
                    }

                    if(lhs_id != -1)
                        symbol_table[lhs_id]->terminal = 0;
                } break;
        }

        buf[0] = 0;
    }

    printf("Read end.\n");

    printf("=== = ==================\n");
    int i = 0;
    while(i < sym_num){
        printf("%3d %d %s\n", i, symbol_table[i]->terminal, symbol_table[i]->name);
        i++;
    }
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

    for(int i = 0; i < sym_num; i++){
        symbol = symbol_table[i];
        if(symbol->terminal == 0)
            printf("%30s %d\n", symbol->name, symbol->nullable);
    }
}

void parser_gen()
{
    //FILE *fp = fopen("./zip/grammar_orig.txt", "r");
    FILE *fp = fopen("G.txt", "r");

    struct Grammar grammar;
    Symbol_List *hash_table[MAX_HASH_BUCKET] = {0};
    Symbol *symbol_table[MAX_SYMBOL] = {0};

    grammar_init(&grammar);
    read_grammar(fp, &grammar, hash_table, symbol_table);
    nullable(&grammar, symbol_table);

    fclose(fp);
}
