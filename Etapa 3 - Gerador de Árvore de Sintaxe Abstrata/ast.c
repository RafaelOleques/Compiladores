// AST - Abstract Syntax Tree

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3)
{
    AST *newnode;
    newnode = (AST*) calloc(1, sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;

    return newnode;
}

void astPrint(AST *node, int level)
{
    if(node == 0)
        return;
    
    int i;
    
    for(i = 0; i <level; i++)
        fprintf(stderr, "  ");

    fprintf(stderr, "ast: ");

    switch (node->type)
    {
    case AST_SYMBOL:
        fprintf(stderr, "AST_SYMBOL");
        break;
    case AST_ADD:
        fprintf(stderr, "AST_ADD");
        break;
    case AST_SUB:
        fprintf(stderr, "AST_SUB");
        break;
    case AST_MULT:
        fprintf(stderr, "AST_MULT");
        break;
    case AST_DIV:
        fprintf(stderr, "AST_DIV");
        break;
    case AST_LESS:
        fprintf(stderr, "AST_LESS");
        break;
    case AST_GREATER:
        fprintf(stderr, "AST_GREATER");
        break;
    case AST_LE:
        fprintf(stderr, "AST_LE");
        break;
    case AST_GE:
        fprintf(stderr, "AST_GE");
        break;
    case AST_EQ:
        fprintf(stderr, "AST_EQ");
        break;
    case AST_DIF:
        fprintf(stderr, "AST_DIF");
        break;
    case AST_chamadaFuncao:
        fprintf(stderr, "AST_chamadaFuncao");
        break;
    case AST_chamadaParametrosEntrada:
        fprintf(stderr, "AST_chamadaParametrosEntrada");
        break;
    case AST_chamadaParametroEntrada:
        fprintf(stderr, "AST_chamadaParametroEntrada");
        break;
    case AST_READ:
        fprintf(stderr, "AST_READ");
        break;
    case AST_atribuicao:
        fprintf(stderr, "AST_atribuicao");
        break;
    case AST_atribuicao_vector:
        fprintf(stderr, "AST_atribuicao_vector");
        break;
    case AST_print:
        fprintf(stderr, "AST_print");
        break;
    case AST_printElementos:
        fprintf(stderr, "AST_printElementos");
        break;
    case AST_printElemento:
        fprintf(stderr, "AST_printElemento");
        break;
    case AST_return:
        fprintf(stderr, "AST_return");
        break;
    case AST_goto:
        fprintf(stderr, "AST_goto");
        break;
    case AST_label:
        fprintf(stderr, "AST_label");
        break;
    case AST_comandoBloco_label:
        fprintf(stderr, "AST_comandoBloco_label");
        break;
    case AST_bloco:
        fprintf(stderr, "AST_bloco");
        break;
    case AST_if:
        fprintf(stderr, "AST_if");
        break;
    case AST_while:
        fprintf(stderr, "AST_while");
        break;
    case AST_comandoBloco:
        fprintf(stderr, "AST_comandoBloco");
        break;
    case AST_VECTOR:
        fprintf(stderr, "AST_VECTOR");
        break;
    case AST_parametroEntradaInt:
        fprintf(stderr, "AST_parametroEntradaInt");
        break;
    case AST_parametroEntradaChar:
        fprintf(stderr, "AST_parametroEntradaChar");
        break;
    case AST_parametroEntradaFloat:
        fprintf(stderr, "AST_parametroEntradaFloat");
        break;
    case AST_parametroEntradaIntMultiplo:
        fprintf(stderr, "AST_parametroEntradaInt");
        break;
    case AST_parametroEntradaCharMultiplo:
        fprintf(stderr, "AST_parametroEntradaCharMultiplo");
        break;
    case AST_parametroEntradaFloatMultiplo:
        fprintf(stderr, "AST_parametroEntradaFloatMultiplo");
        break;
    case AST_cabecalhoFuncaoInt:
        fprintf(stderr, "AST_cabecalhoFuncaoInt");
        break;
    case AST_cabecalhoFuncaoChar:
        fprintf(stderr, "AST_cabecalhoFuncaoChar");
        break;
    case AST_cabecalhoFuncaoFloat:
        fprintf(stderr, "AST_cabecalhoFuncaoFloat");
        break;
    case AST_declaracaoFuncao:
        fprintf(stderr, "AST_declaracaoFuncao");
        break;
    case AST_valoresVetor:
        fprintf(stderr, "AST_valoresVetor");
        break;
    case AST_declaracaoVetorInt:
        fprintf(stderr, "AST_declaracaoVetorInt");
        break;
    case AST_declaracaoVetorChar:
        fprintf(stderr, "AST_declaracaoVetorChar");
        break;
    case AST_declaracaoVetorFloat:
        fprintf(stderr, "AST_declaracaoVetorFloat");
        break;
    case AST_tamVetor:
        fprintf(stderr, "AST_tamVetor");
        break;
    case AST_declaracaoVariavel:
        fprintf(stderr, "AST_declaracaoVariavel");
        break;
    case AST_declaracaoVariavelInt:
        fprintf(stderr, "AST_declaracaoVariavelInt");
        break;
    case AST_declaracaoVariavelChar:
        fprintf(stderr, "AST_declaracaoVariavelChar");
        break;
    case AST_declaracaoVariavelFloat:
        fprintf(stderr, "AST_declaracaoVariavelFloat");
        break;
    case AST_declaracaoVariavelFloatLiteral:
        fprintf(stderr, "AST_declaracaoVariavelFloatLiteral");
        break;
    case AST_decl:
        fprintf(stderr, "AST_decl");
        break;
    case AST_declPV:
        fprintf(stderr, "AST_declPV");
        break;
    case AST_EXPR_PARENTESES:
        fprintf(stderr, "AST_EXPR_PARENTESES");
        break;
    case AST_ifElse:
        fprintf(stderr, "AST_ifElse");
        break;
    case AST_valoresVetorMultiplo:
        fprintf(stderr, "AST_valoresVetorMultiplo");
        break;
    case AST_declaracaoVetorIntValores:
        fprintf(stderr, "AST_declaracaoVetorIntValores");
        break;
    case AST_declaracaoVetorCharValores:
        fprintf(stderr, "AST_declaracaoVetorCharValores");
        break;
    case AST_declaracaoVetorFloatValores:
        fprintf(stderr, "AST_declaracaoVetorFloatValores");
        break;
    
    default:
        fprintf(stderr, "AST_UNKNOWN");
        break;
    }

    if(node->symbol != 0)
        fprintf(stderr, ", %s\n", node->symbol->text);
    else
        fprintf(stderr, ", 0\n");

    
    for(i=0;i<MAX_SONS;i++)
    {
        astPrint(node->son[i], level+1);
    }
}


//END OF FILE

