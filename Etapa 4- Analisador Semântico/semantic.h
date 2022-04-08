#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

#define PILHA_OPERADOR 0
#define PILHA_VETOR 1
#define PILHA_VALOR 2

typedef struct pilha_aritmetica
{
    int tipo;
    int operador;
    float valor;    
    
    struct pilha_aritmetica *expr_esq;
    struct pilha_aritmetica *expr_dir;  

    char* vetor_nome;
    char* indice; //Em caso de vetor
} pilha_aritmetica;

extern int semanticErrors;

void check_program(AST *node);

int getSemanticErrors();

#endif
//END OF FILE