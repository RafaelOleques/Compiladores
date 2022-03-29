#include "semantic.h"
#include <string.h>
#define TRUE 1
#define FALSE 0

FUNCT_PARAMETERS *context;
FUNCT_PARAMETERS *original_context;

int semanticErrors = 0;

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

void set_function_parameters(AST *node, FUNCT_PARAMETERS **actual)
{
    FUNCT_PARAMETERS parameters_aux;
    if (node == 0)
        return;

    int NEXT_PARAMETER = 0;

    FUNCT_PARAMETERS *parameter_node = malloc(sizeof(*parameter_node));

    switch (node->type)
    {
    case AST_parametroEntradaIntMultiplo:
        parameter_node->text = malloc(sizeof(parameter_node->text));

        strcpy(parameter_node->text, node->symbol->text);
        parameter_node->datatype = DATATYPE_INT;

        *actual = parameter_node;

        set_function_parameters(node->son[NEXT_PARAMETER], &((*actual)->next));

        return;

    case AST_parametroEntradaCharMultiplo:
        parameter_node->text = malloc(sizeof(parameter_node->text));

        strcpy(parameter_node->text, node->symbol->text);
        parameter_node->datatype = DATATYPE_CHAR;

        *actual = parameter_node;

        set_function_parameters(node->son[NEXT_PARAMETER], &((*actual)->next));

        return;

    case AST_parametroEntradaFloatMultiplo:
        parameter_node->text = malloc(sizeof(parameter_node->text));

        strcpy(parameter_node->text, node->symbol->text);
        parameter_node->datatype = DATATYPE_FLOAT;

        *actual = parameter_node;

        set_function_parameters(node->son[NEXT_PARAMETER], &((*actual)->next));

        return;

    case AST_parametroEntradaInt:
        parameter_node->text = malloc(sizeof(parameter_node->text));

        strcpy(parameter_node->text, node->symbol->text);
        parameter_node->datatype = DATATYPE_INT;

        *actual = parameter_node;
        (*actual)->next = NULL;

        return;

    case AST_parametroEntradaChar:
        parameter_node->text = malloc(sizeof(parameter_node->text));

        strcpy(parameter_node->text, node->symbol->text);
        parameter_node->datatype = DATATYPE_CHAR;

        *actual = parameter_node;
        (*actual)->next = NULL;

        return;

    case AST_parametroEntradaFloat:
        parameter_node->text = malloc(sizeof(parameter_node->text));

        strcpy(parameter_node->text, node->symbol->text);
        parameter_node->datatype = DATATYPE_FLOAT;

        *actual = parameter_node;
        (*actual)->next = NULL;

        return;

    default:
        return;
    }
}

void set_function_cabecalho(AST *node)
{
    int PARAMETERS = 0;

    if (node->symbol)
        if (node->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "Semantic ERROR: %s are alredy declared, can't be used to FUNCTION NAME\n", node->symbol->text);
            semanticErrors++;
        }

    node->symbol->type = SYMBOL_FUNCTION;
    node->symbol->is_context = FALSE;
    node->son[PARAMETERS]->symbol->parameters = NULL;
    set_function_parameters(node->son[PARAMETERS], &(node->symbol->parameters));
}

void set_context(AST *node)
{
    int i;
    if (context == NULL)
        return;

    FUNCT_PARAMETERS *i_context;

    if (node == 0)
        return;

    if (node->symbol)
    {
        i_context = context;

        while (i_context != NULL)
        {
            if (strcmp(i_context->text, node->symbol->text) == 0)
            {
                FUNCT_PARAMETERS *new_original;
                FUNCT_PARAMETERS *aux;
                new_original = (FUNCT_PARAMETERS *)calloc(1, sizeof(FUNCT_PARAMETERS));

                // Guarda as informações fora do contexto
                new_original->datatype = node->symbol->datatype;
                new_original->type = node->symbol->type;

                new_original->text = (char *)calloc(1, sizeof(char));
                strcpy(new_original->text, node->symbol->text);
                new_original->next = NULL;

                aux = original_context;
                original_context = new_original;
                original_context->next = aux;

                node->symbol->datatype = i_context->datatype;
                node->symbol->type = SYMBOL_VARIABLE;
                node->symbol->is_context = TRUE;

                break;
            }

            i_context = i_context->next;
        }
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        set_context(node->son[i]);
    }
}

void unset_context(AST *node)
{
    int i;
    if (original_context == NULL)
        return;

    FUNCT_PARAMETERS *i_context;

    if (node == 0)
        return;

    if (node->symbol)
    {
        i_context = original_context;

        while (i_context != NULL)
        {
            if (strcmp(i_context->text, node->symbol->text) == 0)
            {
                node->symbol->datatype = i_context->datatype;
                node->symbol->type = i_context->type;
                node->symbol->is_context = FALSE;

                break;
            }

            i_context = i_context->next;
        }
    }

    for (i = 0; i < MAX_SONS; i++)
    {
        unset_context(node->son[i]);
    }
}

void clear_context()
{
    FUNCT_PARAMETERS *i_original_context = original_context;
    FUNCT_PARAMETERS *i_context = context;

    while (i_original_context != NULL)
    {
        free(i_original_context);
        original_context = original_context->next;
        i_original_context = original_context;
    }

    while (i_context != NULL)
    {
        free(i_context);
        context = context->next;
        i_context = context;
    }
}

void check_and_set_declarations(AST *node)
{
    int i;

    if (node == 0)
        return;

    set_datatype(node);

    switch (node->type)
    {
        // case AST_declaracaoFuncao:
    case AST_cabecalhoFuncaoInt:
    case AST_cabecalhoFuncaoChar:
    case AST_cabecalhoFuncaoFloat:
        set_function_cabecalho(node);
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
        node->symbol->is_context = FALSE;
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
        node->symbol->is_context = FALSE;
        break;

    case AST_label:
        if (node->symbol)
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "Semantic ERROR: variable %s are alredy declared\n", node->symbol->text);
                semanticErrors++;
            }
        node->symbol->type = SYMBOL_LABLE;
        node->symbol->is_context = FALSE;
        break;

    default:
        break;

        // case AST_declaracaoVariavelFloatLiteral:
    }
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

// Gera pilha de valores
void gera_pilha(AST *node, int level)
{
    if (node == 0)
        return;

    int i;

    for (i = 0; i < level; i++)
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

    if (node->symbol != 0)
        fprintf(stderr, ", %s\n", node->symbol->text);
    else
        fprintf(stderr, ", 0\n");

    for (i = 0; i < MAX_SONS; i++)
    {
        astPrint(node->son[i], level + 1);
    }
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
int is_exprAritmetica(AST *node, int accept_bool, int accept_float, char *function_call_name)
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

        if ((node->symbol->type == SYMBOL_LIT_FLOAT) ||
            (node->symbol->type == SYMBOL_VARIABLE && node->symbol->datatype == DATATYPE_FLOAT))
            return TRUE;

        return FALSE;

    // Funções
    case AST_chamadaFuncao:
        if (node->symbol->type == SYMBOL_FUNCTION &&
            (node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_FLOAT ||
             node->symbol->datatype == DATATYPE_CHAR))
            return TRUE;
        return FALSE;

    default:
        return FALSE;
    }
}

void expr_check_operands(AST *node, int accept_bool, int accept_float, char *function_call_name)
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
        if (!is_exprAritmetica(node->son[LEFT_OPERAND], accept_bool, accept_float, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid left operand for %s at %s\n", expr_operand_name(node->type), function_call_name);
            semanticErrors++;
        }
        if (!is_exprAritmetica(node->son[RIGHT_OPERAND], accept_bool, accept_float, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid right operand for %s at %s!\n", expr_operand_name(node->type), function_call_name);
            semanticErrors++;
        }
        break;

    case AST_LESS:
    case AST_GREATER:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND], accept_bool, accept_float, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid left operand for %s\n", expr_operand_name(node->type));
            semanticErrors++;
        }
        if (!is_exprAritmetica(node->son[RIGHT_OPERAND], accept_bool, accept_float, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid right operand for %s\n", expr_operand_name(node->type));
            semanticErrors++;
        }
        break;

    default:
        break;
    }
}

void expr_check_parenteses(AST *node, int accept_bool, int accept_float, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_EXPR_PARENTESES:
        if (!is_exprAritmetica(node->son[LEFT_OPERAND], accept_bool, accept_float, function_call_name))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PARENTESES\n");
            semanticErrors++;
        }
        break;

    default:
        break;
    }
}

// Assinatura
void check_arithmetic_expression(AST *, int, int, char *);

int check_chamada_parametros_entrada(AST *node, int num_param_expect, int num_param_used)
{
    if (node == 0)
        return num_param_used;

    int accept_bool = FALSE, accept_float = TRUE;
    int LEFT_SON = 0, RIGHT_SON = 1;
    int counter = num_param_used;

    counter++;

    if (node->type == AST_chamadaParametroEntrada)
    {
        if (!is_exprAritmetica(node->son[LEFT_SON], accept_bool, accept_float, "FUNCTION PARAMETERS"))

        {
            fprintf(stderr, "Semantic ERROR: invalid operand for FUNCTION PARAMETERS!\n");
            semanticErrors++;
        }
        // check_arithmetic_expression(node->son[LEFT_SON], accept_bool, accept_float, "FUNCTION PARAMETERS");

        counter = check_chamada_parametros_entrada(node->son[RIGHT_SON], num_param_expect, counter);
    }
    else
    {
        if (!is_exprAritmetica(node, accept_bool, accept_float, "FUNCTION PARAMETERS"))

        {
            fprintf(stderr, "Semantic ERROR: invalid operand for FUNCTION PARAMETERS\n");
            semanticErrors++;
        }
        // check_arithmetic_expression(node, accept_bool, "FUNCTION PARAMETERS");
    }

    return counter;
}

void expr_check_function_call(AST *node, char *function_call_name)
{
    int i;
    int PARAM_OPERAND = 1;
    int num_param = 0, num_param_used;
    FUNCT_PARAMETERS *i_param;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_chamadaFuncao:
        if (!(node->symbol->type == SYMBOL_FUNCTION &&
              ((node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_FLOAT ||
                node->symbol->datatype == DATATYPE_CHAR))))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for FUNCTION\n", node->symbol->text);
            semanticErrors++;
        }

        i_param = node->symbol->parameters;

        while (i_param != NULL)
        {
            num_param++;
            i_param = i_param->next;
        }
        num_param_used = check_chamada_parametros_entrada(node->son[PARAM_OPERAND], num_param, 0);

        if (num_param != num_param_used)
        {
            fprintf(stderr, "Semantic ERROR: expected %d parameter for FUNCTION '%s', used %d\n", num_param, node->symbol->text, num_param_used);
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
        if (!(node->symbol->type == SYMBOL_LIT_CHAR || node->symbol->type == SYMBOL_LIT_FLOAT ||
              node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_VARIABLE))
        {
            fprintf(stderr, "Semantic ERROR: invalid \'%s\' for SYMBOL\n", node->symbol->text);
            semanticErrors++;
        }
        else if (node->symbol->type == SYMBOL_VARIABLE &&
                 !(node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR || node->symbol->datatype == DATATYPE_FLOAT))
        {
            fprintf(stderr, "Semantic ERROR: invalid variable '%s\' TYPE\n", node->symbol->text);
            semanticErrors++;
        }

        break;

    default:
        break;
    }
}

void check_arithmetic_expression(AST *node, int accept_bool, int accept_float, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0, RIGHT_OPERAND = 1;

    if (node == 0)
        return;

    expr_check_operands(node, accept_bool, accept_float, function_call_name);
    expr_check_parenteses(node, accept_bool, accept_float, function_call_name);
    expr_check_function_call(node, function_call_name);
    expr_check_variable_literal_call(node);

    for (i = 0; i < MAX_SONS; i++)
    {
        /*-
        // printf("===> check_arithmetic_expression for\n");
        printf("\n==============================\n");
        gera_pilha(node->son[i], 0);
        printf("==============================\n\n");*/

        check_arithmetic_expression(node->son[i], accept_bool, accept_float, function_call_name);
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
        case DATATYPE_CHAR:
        case DATATYPE_FLOAT:
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
        case DATATYPE_CHAR:
        case DATATYPE_FLOAT:
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
    int accept_bool = FALSE, accept_float = TRUE;

    if (!is_variable(node->symbol))
    {
        fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for VARIABLE ASSEGMENT\n", node->symbol->text);
        semanticErrors++;
    }
    if (!is_exprAritmetica(node->son[FIRST_OPERAND], accept_bool, accept_float, "VARIABLE VALUE"))
    {
        fprintf(stderr, "Semantic ERROR: invalid right operand for VARIABLE ASSEGMENT\n");
        semanticErrors++;
    }
    // printf("===> %s\n",  node->symbol->text);
    printf("\n==============================\n");
        gera_pilha(node->son[FIRST_OPERAND], 0);
        printf("==============================\n\n");

    check_arithmetic_expression(node->son[FIRST_OPERAND], accept_bool, accept_float, "VARIABLE VALUE");
}

void atrib_vector(AST *node)
{
    int INDEX = 0, VALUE = 1;
    int accept_bool = FALSE, accept_float = TRUE;

    // Identifier
    if (!is_vector(node->symbol))
    {
        fprintf(stderr, "Semantic ERROR: invalid identifier \'%s\' for VECTOR ASSEGMENT\n", node->symbol->text);
        semanticErrors++;
    }

    // Index
    if (!is_exprAritmetica(node->son[INDEX], accept_bool, !accept_float, "VECTOR INDEX"))
    {
        fprintf(stderr, "Semantic ERROR: invalid index operand for VECTOR ASSEGMENT\n");
        semanticErrors++;
    }
    // printf("===> atrib_vector index\n");

    printf("\n==============================\n");
        gera_pilha(node->son[INDEX], 0);
        printf("==============================\n\n");

    check_arithmetic_expression(node->son[INDEX], accept_bool, !accept_float, "VECTOR INDEX");

    // Value
    if (!is_exprAritmetica(node->son[VALUE], accept_bool, accept_float, "VECTOR VALUE"))
    {
        fprintf(stderr, "Semantic ERROR: invalid value operand for VECTOR ASSEGMENT\n");
        semanticErrors++;
    }
    // printf("===> atrib_vector value\n");
    printf("\n==============================\n");
        gera_pilha(node->son[VALUE], 0);
        printf("==============================\n\n");

    check_arithmetic_expression(node->son[VALUE], accept_bool, accept_float, "VECTOR VALUE");
}

void check_atrib(AST *node)
{
    int i;
    int accept_bool = FALSE, accept_float = TRUE;

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
    int accept_bool = FALSE, accept_float = TRUE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_return:
        if (!is_exprAritmetica(node->son[FIRST_OPERAND], accept_bool, accept_float, "RETURN"))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for RETURN\n");
            semanticErrors++;
        }

        printf("\n==============================\n");
        gera_pilha(node->son[FIRST_OPERAND], 0);
        printf("==============================\n\n");

        check_arithmetic_expression(node->son[FIRST_OPERAND], accept_bool, accept_float, "RETURN");

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

void check_print_elemento(AST *node, int accept_bool, int accept_float)
{
    switch (node->type)
    {
    case AST_SYMBOL:
        if (
            !(node->symbol->type == SYMBOL_LIT_STRING ||
              is_exprAritmetica(node, accept_bool, accept_float, "PRINT")))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PRINT\n");
            semanticErrors++;
        }
        break;

    default:
        if (!is_exprAritmetica(node, accept_bool, accept_float, "PRINT"))

        {
            fprintf(stderr, "Semantic ERROR: invalid operand for PRINT\n");
            semanticErrors++;
        }
        
        printf("\n==============================\n");
        gera_pilha(node, 0);
        printf("==============================\n\n");

        check_arithmetic_expression(node, accept_bool, accept_float, "PRINT");
        break;
    }
}

void check_print_elementos(AST *node, int accept_bool, int accept_float)
{
    int LEFT_SON = 0, RIGHT_SON = 1;

    switch (node->type)
    {
    case AST_printElementos:
        check_print_elemento(node->son[LEFT_SON], accept_bool, accept_float);
        check_print_elementos(node->son[RIGHT_SON], accept_bool, accept_float);
        break;

    default:
        check_print_elemento(node, accept_bool, accept_float);
    }
}

void check_print(AST *node)
{
    int i;
    int FIRST_OPERAND = 0;
    int accept_bool = FALSE, accept_float = TRUE;

    if (node == 0)
        return;

    if (node->type == AST_print)
    {
        check_print_elementos(node->son[FIRST_OPERAND], accept_bool, accept_float);
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

// **********************
// Label
// **********************

void check_label(AST *node)
{
    if (node == 0)
        return;

    if (!(node->symbol->type == SYMBOL_LABLE))
    {
        fprintf(stderr, "Semantic ERROR: invalid LABEL\n");
        semanticErrors++;
    }
}

// Assinaturas

void check_comando(AST *node);
void check_comando_bloco(AST *node);

// **********************
// Comando Simples
// **********************

void check_comando_simples(AST *node)
{
    if (node == 0)
        return;

    check_atrib(node);
    // check_controle_fluxo(node);
    check_print(node);
    check_return(node);
}

// **********************
// Comando Bloco
// **********************

void check_comando_bloco(AST *node)
{
    int FIRST_SON = 0, SECOND_SON = 1;
    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_comandoBloco:
        check_comando(node->son[FIRST_SON]);
        check_comando_bloco(node->son[SECOND_SON]);
        break;

    case AST_comandoBloco_label:
        check_label(node->son[FIRST_SON]);
        check_comando_bloco(node->son[SECOND_SON]);
        break;

    case AST_label:
        check_label(node);
        break;
    default:
        break;
    }
}

// **********************
// Bloco
// **********************

void check_bloco(AST *node)
{
    int FIRST_SON = 0;

    if (node == 0)
        return;

    if (node->type == AST_bloco)
        check_comando_bloco(node->son[FIRST_SON]);
}

// **********************
// Comando
// **********************

void check_comando(AST *node)
{
    switch (node->type)
    {
    case AST_bloco:
        check_bloco(node);
        break;

    default:
        check_comando_simples(node);
        break;
    }
}

// ************************************
// Paramêtro de entrada de uma função
// ************************************

int is_identifier(HASH_NODE *node)
{

    if (node->type == SYMBOL_IDENTIFIER)
        return TRUE;
    else
        return FALSE;
}

void check_parametro_entrada(AST *node)
{
    if (node == 0)
        return;

    int FIRST_SON = 0;

    switch (node->type)
    {
    case AST_parametroEntradaIntMultiplo:
    case AST_parametroEntradaCharMultiplo:
    case AST_parametroEntradaFloatMultiplo:
        if (!is_identifier(node->symbol))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for FUNCTION PARAMETER\n", node->symbol->text);
            semanticErrors++;
        }
        check_parametro_entrada(node->son[FIRST_SON]);
        break;

    case AST_parametroEntradaInt:
    case AST_parametroEntradaChar:
    case AST_parametroEntradaFloat:
        if (!is_identifier(node->symbol))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for FUNCTION PARAMETER\n", node->symbol->text);
            semanticErrors++;
        }
        break;

    default:
        break;
    }
}

// ************************************
// Paramêtros de entrada de uma função
// ************************************

void check_parametros_entrada(AST *node)
{
    if (node == 0)
        return;

    check_parametro_entrada(node);
}

// **********************
// Cabecalho de função
// **********************

int is_cabecalho_funcao(HASH_NODE *node)
{
    switch (node->type)
    {
    case SYMBOL_FUNCTION:
        switch (node->datatype)
        {
        case DATATYPE_INT:
        case DATATYPE_CHAR:
        case DATATYPE_FLOAT:
            return TRUE;
        default:
            break;
        }

    default:
        return FALSE;
    }
}

FUNCT_PARAMETERS *check_cabecalho_funcao(AST *node)
{
    if (node == 0)
        return NULL;

    FUNCT_PARAMETERS *funct_context = NULL;

    int FIRST_SON = 0;

    switch (node->type)
    {
    case AST_cabecalhoFuncaoInt:
    case AST_cabecalhoFuncaoChar:
    case AST_cabecalhoFuncaoFloat:
        if (!is_cabecalho_funcao(node->symbol))
        {
            fprintf(stderr, "Semantic ERROR: invalid operand \'%s\' for FUNCTION HEADER\n", node->symbol->text);
            semanticErrors++;
        }
        else
            funct_context = node->symbol->parameters;

        check_parametros_entrada(node->son[FIRST_SON]);

    default:
        break;
    }

    return funct_context;
}

// **********************
// Declaração de função
// **********************

void check_declaracao_funcao(AST *node)
{
    if (node == 0)
        return;

    int FIRST_SON = 0, SECOND_SON = 1;

    if (node->type == AST_declaracaoFuncao)
    {
        original_context = NULL;

        context = check_cabecalho_funcao(node->son[FIRST_SON]);

        set_context(node->son[SECOND_SON]);
        check_comando(node->son[SECOND_SON]);
        unset_context(node->son[SECOND_SON]);

        clear_context();
    }
    else
    {
        fprintf(stderr, "Semantic ERROR: expect a function declaration\n");
        semanticErrors++;
    }
}

// ************
// Declaração
// ************

void check_decl(AST *node)
{
    if (node == 0)
        return;

    int FIRST_SON = 0, SECOND_SON = 1;

    switch (node->type)
    {
    case AST_decl:
        check_declaracao_funcao(node->son[FIRST_SON]);
        check_decl(node->son[SECOND_SON]);
        break;
    case AST_declPV:
        // check_declaracao_variavel(node->son[FIRST_SON]);
        check_decl(node->son[SECOND_SON]);
        break;

    default:
        break;
    }
}

// _____________________________________________________ //

void check_program(AST *node)
{
    check_and_set_declarations(node);
    printf("\n");
    // Retirar esse, verificação dentro das funções
    // check_undeclared();
    printf("\n");

    check_decl(node);
    check_goto(node);
}

int getSemanticErrors()
{
    return semanticErrors;
}
