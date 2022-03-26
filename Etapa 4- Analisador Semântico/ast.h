// AST - Abstract Syntax Tree

#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAX_SONS 4

#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_MULT 4
#define AST_DIV 5
#define AST_LESS 6
#define AST_GREATER 7
#define AST_LE 8
#define AST_GE 9
#define AST_EQ 10
#define AST_DIF 11
#define AST_chamadaFuncao 12
#define AST_chamadaParametrosEntrada 13
#define AST_chamadaParametroEntrada 14
#define AST_READ 15
#define AST_atribuicao 16
#define AST_atribuicao_vector 17
#define AST_print 18
#define AST_printElementos 19
#define AST_printElemento 20
#define AST_return 21
#define AST_goto 22
#define AST_label 23
#define AST_comandoBloco_label 24
#define AST_bloco 25
#define AST_if 26
#define AST_while 27
#define AST_comandoBloco 28
#define AST_VECTOR 29
#define AST_parametroEntradaInt 30
#define AST_parametroEntradaChar 31
#define AST_parametroEntradaFloat 32
#define AST_cabecalhoFuncaoInt 33
#define AST_cabecalhoFuncaoChar 34
#define AST_cabecalhoFuncaoFloat 35
#define AST_declaracaoFuncao 36
#define AST_valoresVetor 37
#define AST_declaracaoVetorInt 38
#define AST_declaracaoVetorChar 39
#define AST_declaracaoVetorFloat 40
#define AST_tamVetor 41
#define AST_declaracaoVariavel 42
#define AST_declaracaoVariavelInt 43
#define AST_declaracaoVariavelChar 44
#define AST_declaracaoVariavelFloat 45
#define AST_declaracaoVariavelFloatLiteral 46
#define AST_decl 47
#define AST_declPV 48
#define AST_EXPR_PARENTESES 49
#define AST_ifElse 50
#define AST_parametroEntradaIntMultiplo 51
#define AST_parametroEntradaCharMultiplo 52
#define AST_parametroEntradaFloatMultiplo 53
#define AST_valoresVetorMultiplo 54
#define AST_declaracaoVetorIntValores 55
#define AST_declaracaoVetorCharValores 56
#define AST_declaracaoVetorFloatValores 57


#define AST_UNKNOWN 0

typedef struct astnode
{
    int type;
    HASH_NODE *symbol;
    struct astnode *son[MAX_SONS];
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST *node, int level);

#endif
//END OF FILE

