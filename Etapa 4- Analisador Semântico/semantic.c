#include "semantic.h"
#define TRUE 1
#define FALSE 0

int semanticErrors = 0;

void set_type(AST *node)
{
    switch (node->type)
    {
    // INT
    case AST_declaracaoVariavelInt:
    case AST_cabecalhoFuncaoInt:
    case AST_declaracaoVetorInt:
    case AST_declaracaoVetorIntValores:
        node->symbol->datatype = DATATYPE_INT;
        break;

    // CHAR
    case AST_declaracaoVariavelChar:
    case AST_cabecalhoFuncaoChar:
    case AST_declaracaoVetorChar:
    case AST_declaracaoVetorCharValores:
        node->symbol->datatype = DATATYPE_CHAR;
        break;

    // FLOAT
    case AST_declaracaoVariavelFloat:
    case AST_declaracaoVetorFloatValores:
    case AST_cabecalhoFuncaoFloat:
    case AST_declaracaoVetorFloat:
        node->symbol->datatype = DATATYPE_FLOAT;
        break;
    }
}

void check_and_set_declarations(AST *node)
{
    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
        // case AST_declaracaoFuncao:
    case AST_cabecalhoFuncaoInt:
    case AST_cabecalhoFuncaoChar:
    case AST_cabecalhoFuncaoFloat:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic ERROR: function %s are alredy declared\n", node->symbol->text);
                semanticErrors++;
            }

        node->symbol->type = SYMBOL_FUNCTION;
        break;

    case AST_declaracaoVetorInt:
    case AST_declaracaoVetorChar:
    case AST_declaracaoVetorFloat:
    case AST_declaracaoVetorIntValores:
    case AST_declaracaoVetorCharValores:
    case AST_declaracaoVetorFloatValores:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic ERROR: vector %s are alredy declared\n", node->symbol->text);
                semanticErrors++;
            }

        node->symbol->type = SYMBOL_VECTOR;
        break;

    // case AST_declaracaoVariavel:
    case AST_declaracaoVariavelInt:
    case AST_declaracaoVariavelChar:
    case AST_declaracaoVariavelFloat:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic ERROR: variable %s are alredy declared\n", node->symbol->text);
                semanticErrors++;
            }

        node->symbol->type = SYMBOL_VARIABLE;
        break;

    default:
        break;

        // case AST_declaracaoVariavelFloatLiteral:
    }

    set_type(node);

    for (i = 0; i < MAX_SONS; i++)
    {
        check_and_set_declarations(node->son[i]);
    }
}

void check_undeclared()
{
    semanticErrors += hash_check_undeclared();
}

int is_exprAritmetica(AST *node)
{
    if (node == 0)
    {
        return FALSE;
    }
    switch (node->type)
    {

    // Operadores
    case AST_ADD:
    case AST_SUB:
    case AST_MULT:
    case AST_DIV:
    case AST_LESS:
    case AST_GREATER:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
        return TRUE;

    //Especiais
    case AST_READ:
    case AST_EXPR_PARENTESES:
        return TRUE;

    // Literais e variáveis
    case AST_SYMBOL:
        if (node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CHAR ||
            node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR)
            return TRUE;
        return FALSE;

    //Falta vetor

    // Funções
    case AST_chamadaFuncao:
        if (node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR)
            return TRUE;
        return FALSE;

    default:
        return FALSE;
    }
}

char *operand_name(int type)
{
    switch (type)
    {
    case AST_ADD:
        return "ADD";
    case AST_SUB:
        return "SUBB";
    case AST_MULT:
        return "MULT";
    case AST_DIV:
        return "DIV";
    case AST_LESS:
        return "LESS";
    case AST_GREATER:
        return "GREATER";
    case AST_LE:
        return "LESS EQUAL";
    case AST_GE:
        return "GREATER EQUAL";
    case AST_EQ:
        return "EQUAL";
    case AST_DIF:
        return "DIFF";
    default:
        return "NONE";
    }
}

void check_operands(AST *node)
{
    int i;
    int LEFT_OPERAND = 0, RIGHT_OPERAND = 1;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_ADD:
    case AST_SUB:
    case AST_MULT:
    case AST_DIV:
    case AST_LESS:
    case AST_GREATER:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND]))
        {
            fprintf(stderr, "Semantic ERROR: invalid left operand for %s\n", operand_name(node->type));
            semanticErrors++;
        }
        if (!is_exprAritmetica(node->son[RIGHT_OPERAND]))
        {
            fprintf(stderr, "Semantic ERROR: invalid right operand for %s\n", operand_name(node->type));
            semanticErrors++;
        }
        break;

    case AST_EXPR_PARENTESES:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND]))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PARENTESES\n");
            semanticErrors++;
        }

    default:
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        check_operands(node->son[i]);
    }
}

int getSemanticErrors()
{
    return semanticErrors;
}
