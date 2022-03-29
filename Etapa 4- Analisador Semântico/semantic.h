#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

typedef struct pilha_aritmetica
{
    char* identificador;
    char* indice; //Em caso de vetor
    struct pilha_aritmetica *next;    
} pilha_aritmetica;

extern int semanticErrors;

void check_and_set_declarations(AST *node);
void check_undeclared();
void check_program(AST *node);

int getSemanticErrors();

#endif
//END OF FILE