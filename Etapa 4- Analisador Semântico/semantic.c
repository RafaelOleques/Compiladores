#include "semantic.h"
#include <string.h>
#define TRUE 1
#define FALSE 0

int semanticErrors = 0;

// Falta os parâmetros da função
void set_datatype(AST *node)
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

FUNCT_PARAMETERS *set_function_parameters(AST *node)
{
    if (node == 0)
        return 0;
    printf("1 %s!\n", node->symbol->text);

    int NEXT_PARAMETER = 0;

    switch (node->type)
    {
    case AST_parametroEntradaIntMultiplo:
        printf("AST_parametroEntradaIntMultiplo!\n");
        node->symbol->parameters = (FUNCT_PARAMETERS*) calloc(1, sizeof(FUNCT_PARAMETERS));
        node->symbol->parameters->text = (char*) calloc(1, sizeof(char));

        strcpy(node->symbol->parameters->text, node->symbol->text);

        node->symbol->parameters->datatype = DATATYPE_INT;
        node->symbol->parameters->next = set_function_parameters(node->son[NEXT_PARAMETER]);
        
        return node->symbol->parameters;

    case AST_parametroEntradaCharMultiplo:
        printf("AST_parametroEntradaCharMultiplo %s!\n", node->symbol->text);
        node->symbol->parameters = (FUNCT_PARAMETERS*) calloc(1, sizeof(FUNCT_PARAMETERS));
        node->symbol->parameters->text = (char*) calloc(1, sizeof(char));

        strcpy(node->symbol->parameters->text, node->symbol->text);

        node->symbol->parameters->datatype = DATATYPE_CHAR;
        node->symbol->parameters->next = set_function_parameters(node->son[NEXT_PARAMETER]);
        
        return node->symbol->parameters;

    case AST_parametroEntradaFloatMultiplo:
        printf("AST_parametroEntradaFloatMultiplo %s!\n", node->symbol->text);
        node->symbol->parameters = (FUNCT_PARAMETERS*) calloc(1, sizeof(FUNCT_PARAMETERS));
        node->symbol->parameters->text = (char*) calloc(1, sizeof(char));

        strcpy(node->symbol->parameters->text, node->symbol->text);

        node->symbol->parameters->datatype = DATATYPE_FLOAT;
        node->symbol->parameters->next = set_function_parameters(node->son[NEXT_PARAMETER]);
        
        return node->symbol->parameters;

    case AST_parametroEntradaInt:
        printf("AST_parametroEntradaInt %s!\n", node->symbol->text);
        node->symbol->parameters = (FUNCT_PARAMETERS*) calloc(1, sizeof(FUNCT_PARAMETERS));
        node->symbol->parameters->text = (char*) calloc(1, sizeof(char));

        strcpy(node->symbol->parameters->text, node->symbol->text);
        node->symbol->parameters->datatype = DATATYPE_INT;
        node->symbol->parameters->next = 0;

        return node->symbol->parameters;

    case AST_parametroEntradaChar:
        printf("AST_parametroEntradaChar %s!\n", node->symbol->text);
        node->symbol->parameters = (FUNCT_PARAMETERS*) calloc(1, sizeof(FUNCT_PARAMETERS));
        node->symbol->parameters->text = (char*) calloc(1, sizeof(char));

        strcpy(node->symbol->parameters->text, node->symbol->text);
        node->symbol->parameters->datatype = DATATYPE_CHAR;
        node->symbol->parameters->next = 0;
        
        return node->symbol->parameters;

    case AST_parametroEntradaFloat:
        printf("AST_parametroEntradaFloat %s!\n", node->symbol->text);
        node->symbol->parameters = (FUNCT_PARAMETERS*) calloc(1, sizeof(FUNCT_PARAMETERS));
        node->symbol->parameters->text = (char*) calloc(1, sizeof(char));

        strcpy(node->symbol->parameters->text, node->symbol->text);
        node->symbol->parameters->datatype = DATATYPE_FLOAT;
        node->symbol->parameters->next = 0;
        
        return node->symbol->parameters;

    default:
        printf("default %s!\n", node->symbol->text);
        return 0;
    }
}

void print_function_parameters(FUNCT_PARAMETERS *parameters)
{
    if (parameters == 0)
        return;


    printf("%s: ", parameters->text);

    switch (parameters->datatype)
    {
    case DATATYPE_CHAR:
        printf("CHAR\n");
        break;
    case DATATYPE_FLOAT:
        printf("FLOAT\n");
        break;
    case DATATYPE_INT:
        printf("INT\n");
        break;

    default:
        break;
    }

    print_function_parameters(parameters->next);
}

void check_and_set_declarations(AST *node)
{
    int i;
    int PARAMETERS = 0;

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
        set_function_parameters(node->son[PARAMETERS]);

        printf("===================================\n");
        printf("FUNCTION: %s\n", node->symbol->text);
        printf("PARAMETERS:\n");
        print_function_parameters(node->son[PARAMETERS]->symbol->parameters);
        printf("===================================\n");
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

    case AST_label:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic ERROR: variable %s are alredy declared\n", node->symbol->text);
                semanticErrors++;
            }
        node->symbol->type = SYMBOL_LABLE;
        break;

    default:
        break;

        // case AST_declaracaoVariavelFloatLiteral:
    }

    set_datatype(node);
    // set_struct(node);

    for (i = 0; i < MAX_SONS; i++)
    {
        check_and_set_declarations(node->son[i]);
    }
}

void check_undeclared()
{
    semanticErrors += hash_check_undeclared();
}

// **********************
// Expressões Aritméticas
// **********************

char *expr_operand_name(int type)
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

// Falta vetor e parâmetros de função
int is_exprAritmetica(AST *node, int accept_bool, char *function_call_name)
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
        return TRUE;

    case AST_LESS:
    case AST_GREATER:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
        if (!accept_bool)
        {
            semanticErrors += 1;
            fprintf(stderr, "SEMANTIC ERROR: Bool operand %s not accept at %s\n", expr_operand_name(node->type), function_call_name);
        }
        return accept_bool;

    // Especiais
    case AST_READ:
    case AST_EXPR_PARENTESES:
        return TRUE;

    // Literais e variáveis
    case AST_SYMBOL:
        if (
            (node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CHAR) ||
            (node->symbol->type == SYMBOL_VARIABLE &&
             (node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR)))
            return TRUE;
        return FALSE;

    // Funções
    case AST_chamadaFuncao:
        if (node->symbol->type == SYMBOL_FUNCTION &&
            (node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR))
            return TRUE;
        return FALSE;

    default:
        return FALSE;
    }
}

void expr_check_operands(AST *node, int accept_bool, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0, RIGHT_OPERAND = 1;

    if (node == 0)
        return;

    switch (node->type)
    {

    // Operandos
    case AST_ADD:
    case AST_SUB:
    case AST_MULT:
    case AST_DIV:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND], accept_bool, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid left operand for %s\n", expr_operand_name(node->type));
            semanticErrors++;
        }
        if (!is_exprAritmetica(node->son[RIGHT_OPERAND], accept_bool, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid right operand for %s\n", expr_operand_name(node->type));
            semanticErrors++;
        }
        break;

    case AST_LESS:
    case AST_GREATER:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND], accept_bool, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid left operand for %s\n", expr_operand_name(node->type));
            semanticErrors++;
        }
        if (!is_exprAritmetica(node->son[RIGHT_OPERAND], accept_bool, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid right operand for %s\n", expr_operand_name(node->type));
            semanticErrors++;
        }
        break;

    default:
        break;
    }
}

void expr_check_parenteses(AST *node, int accept_bool, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_EXPR_PARENTESES:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND], accept_bool, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PARENTESES\n");
            semanticErrors++;
        }
        break;

    default:
        break;
    }
}

// Falta verificar os parâmetros
void expr_check_function_call(AST *node)
{
    int i;
    int LEFT_OPERAND = 0;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_chamadaFuncao:
        if (!(node->symbol->type == SYMBOL_FUNCTION &&
              ((node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR))))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for FUNCTION\n", node->symbol->text);
            semanticErrors++;
        }
        break;

    default:
        break;
    }
}

void expr_check_variable_literal_call(AST *node)
{
    int i;
    int LEFT_OPERAND = 0;
    int error = FALSE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_SYMBOL:
        if (!(node->symbol->type == SYMBOL_LIT_CHAR ||
              node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_VARIABLE))
        {
            fprintf(stderr, "Semantic ERROR: invalid \'%s\' for SYMBOL\n", node->symbol->text);
            semanticErrors++;
        }
        else if (node->symbol->type == SYMBOL_VARIABLE &&
                 !(node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand '%s\' for VARIABLE\n", node->symbol->text);
            semanticErrors++;
        }

        break;

    default:
        break;
    }
}

void check_arithmetic_expression(AST *node, int accept_bool, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0, RIGHT_OPERAND = 1;

    if (node == 0)
        return;

    expr_check_operands(node, accept_bool, function_call_name);
    expr_check_parenteses(node, accept_bool, function_call_name);
    expr_check_function_call(node);
    expr_check_variable_literal_call(node);

    for (i = 0; i < MAX_SONS; i++)
    {
        check_arithmetic_expression(node->son[i], accept_bool, function_call_name);
    }
}

// **********************
// Atribuições
// **********************

int is_variable(HASH_NODE *node)
{
    switch (node->type)
    {
    case SYMBOL_VARIABLE:
        switch (node->datatype)
        {
        case DATATYPE_INT:
            // return DATATYPE_INT;
        case DATATYPE_CHAR:
            // return DATATYPE_CHAR;
            return TRUE;
        default:
            break;
        }

    default:
        return FALSE;
    }
}

int is_vector(HASH_NODE *node)
{
    switch (node->type)
    {
    case SYMBOL_VECTOR:
        switch (node->datatype)
        {
        case DATATYPE_INT:
            // return DATATYPE_INT;
        case DATATYPE_CHAR:
            // return DATATYPE_CHAR;
            return TRUE;
        default:
            break;
        }

    default:
        return FALSE;
    }
}

void atrib_variable(AST *node)
{
    int FIRST_OPERAND = 0;
    int accept_bool = FALSE;

    if (!is_variable(node->symbol))
    {
        fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for VARIABLE ASSEGMENT\n", node->symbol->text);
        semanticErrors++;
    }
    if (!is_exprAritmetica(node->son[FIRST_OPERAND], accept_bool, "VARIABLE VALUE"))
    {
        fprintf(stderr, "Semantic ERROR: invalid right operand for VARIABLE ASSEGMENT\n");
        semanticErrors++;
    }

    check_arithmetic_expression(node->son[FIRST_OPERAND], accept_bool, "VARIABLE VALUE");
}

void atrib_vector(AST *node)
{
    int INDEX = 0, VALUE = 1;
    int accept_bool = FALSE;

    // Identifier
    if (!is_vector(node->symbol))
    {
        fprintf(stderr, "Semantic ERROR: invalid identifier \'%s\' for VECTOR ASSEGMENT\n", node->symbol->text);
        semanticErrors++;
    }

    // Index
    if (!is_exprAritmetica(node->son[INDEX], accept_bool, "VECTOR INDEX"))
    {
        fprintf(stderr, "Semantic ERROR: invalid index operand for VECTOR ASSEGMENT\n");
        semanticErrors++;
    }

    check_arithmetic_expression(node->son[INDEX], accept_bool, "VECTOR INDEX");

    // Value
    if (!is_exprAritmetica(node->son[VALUE], accept_bool, "VECTOR VALUE"))
    {
        fprintf(stderr, "Semantic ERROR: invalid value operand for VECTOR ASSEGMENT\n");
        semanticErrors++;
    }

    check_arithmetic_expression(node->son[VALUE], accept_bool, "VECTOR VALUE");
}

void check_atrib(AST *node)
{
    int i;
    int accept_bool = FALSE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_atribuicao:
        atrib_variable(node);
        break;
    case AST_atribuicao_vector:
        atrib_vector(node);

    default:
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        check_atrib(node->son[i]);
    }
}

// **********************
// Return
// **********************

void check_return(AST *node)
{
    int i;
    int FIRST_OPERAND = 0;
    int accept_bool = FALSE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_return:
        if (!is_exprAritmetica(node->son[FIRST_OPERAND], accept_bool, "RETURN"))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for RETURN\n");
            semanticErrors++;
        }
        check_arithmetic_expression(node->son[FIRST_OPERAND], accept_bool, "RETURN");

    default:
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        check_return(node->son[i]);
    }
}

// **********************
// Print
// **********************

void check_print_elemento(AST *node, int accept_bool)
{
    switch (node->type)
    {
    case AST_SYMBOL:
        if (
            !(node->symbol->type == SYMBOL_LIT_STRING ||
              is_exprAritmetica(node, accept_bool, "PRINT")))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PRINT\n");
            semanticErrors++;
        }
        break;

    default:
        if (!is_exprAritmetica(node, accept_bool, "PRINT"))

        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PRINT\n");
            semanticErrors++;
        }
        check_arithmetic_expression(node, accept_bool, "PRINT");
        break;
    }
}

void check_print_elementos(AST *node, int accept_bool)
{
    int LEFT_SON = 0, RIGHT_SON = 1;

    switch (node->type)
    {
    case AST_printElementos:
        check_print_elemento(node->son[LEFT_SON], accept_bool);
        check_print_elementos(node->son[RIGHT_SON], accept_bool);
        break;

    default:
        check_print_elemento(node, accept_bool);
    }
}

void check_print(AST *node)
{
    int i;
    int FIRST_OPERAND = 0;
    int accept_bool = FALSE;

    if (node == 0)
        return;

    if (node->type == AST_print)
    {
        check_print_elementos(node->son[FIRST_OPERAND], accept_bool);
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        check_print(node->son[i]);
    }
}

// **********************
// Goto
// **********************

void check_goto(AST *node)
{
    int i;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_goto:
        if (!(node->symbol->type == SYMBOL_LABLE))
        {
            fprintf(stderr, "Semantic ERROR: invalid label for GOTO\n");
            semanticErrors++;
        }

    default:
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        check_goto(node->son[i]);
    }
}

// _____________________________________________________ //

void check_program(AST *node)
{
    check_and_set_declarations(node);
    printf("\n");
    check_undeclared();
    printf("\n");

    check_atrib(node);
    check_return(node);
    check_print(node);
    check_goto(node);
}

int getSemanticErrors()
{
    return semanticErrors;
}
