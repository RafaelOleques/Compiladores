#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1

#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MULT 4
#define TAC_DIV 5
#define TAC_LESS 6
#define TAC_GREATER 7
#define TAC_LE 8
#define TAC_GE 9
#define TAC_EQ 10
#define TAC_DIF 11

#define TAC_atribuicao 12

#define TAC_IF 13
#define TAC_JFALSE 14
#define TAC_LABEL 15

#define TAC_READ 16

#define TAC_declaracaoVariavelGeral 17
#define TAC_declaracaoVariavel 18
#define TAC_declaracaoVariavelFloatLiteral 19

#define TAC_declaracaoVetor 20
#define TAC_tamVetor 21
#define TAC_valoresVetor 22

#define TAC_RETURN 23

#define TAC_PRINTELEMENTOS 24
#define TAC_PRINT 25

#define TAC_GOTO 26

#define TAC_PARAMETROSENTRADA 27
#define TAC_CALLFUNCT 28
#define TAC_BEGINFUNT 29
#define TAC_ENDFUNCT 30

#define TAC_atribuicao_vector 31
#define TAC_VECTOR 32



typedef struct tac_node
{
    int type;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
}TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac);
TAC *tacJoin(TAC *l1, TAC *l2);

// CODE GENERATION
TAC *generateCode(AST *node);



#endif
