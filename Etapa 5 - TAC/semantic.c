#include "semantic.h"
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

FUNCT_PARAMETERS *context;
FUNCT_PARAMETERS *original_context;
pilha_aritmetica *pilha;

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
    if( node->son[PARAMETERS])
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

void set_variable(AST *node)
{
    if (node->symbol)
        if (node->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr, "Semantic ERROR: variable %s are alredy declared\n", node->symbol->text);
            semanticErrors++;
        }

    node->symbol->type = SYMBOL_VARIABLE;
    node->symbol->is_context = FALSE;
}

float set_variable_value(char *text)
{
    if (text[0] == '\'')
        return text[1];
    else
        return atoi(text);
}

float set_variable_value_float(char *val1, char *val2)
{
    return set_variable_value(val1) / set_variable_value(val2);
}

int chech_vector_values_total(AST *node, int count)
{
    int FIRST_SON = 0, SECOND_SON = 1;

    if (node->son[SECOND_SON] == 0)
        return count;

    return chech_vector_values_total(node->son[SECOND_SON], count + 1);
}

int chech_vector_values(AST *node)
{
    int total = 0;
    switch (node->type)
    {
    case AST_declaracaoVetorInt:
    case AST_declaracaoVetorChar:
    case AST_declaracaoVetorFloat:
        break;

    case AST_declaracaoVetorIntValores:
    case AST_declaracaoVetorCharValores:
    case AST_declaracaoVetorFloatValores:
        return total = chech_vector_values_total(node->son[1], 1);
        break;
    }

    return -1;
}

void check_and_set_declarations(AST *node)
{
    int i;
    int FIRST_SON = 0, SECOND_SON = 1;
    int total_param;

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
        node->symbol->vector_size = atoi(node->son[FIRST_SON]->symbol->text);

        total_param = chech_vector_values(node);

        if (total_param != -1)
            if (total_param > node->symbol->vector_size)
            {
                fprintf(stderr, "Semantic ERROR: %s wait at most %d values, used %d\n", node->symbol->text, node->symbol->vector_size, total_param);
                semanticErrors++;
            }
        break;

    case AST_declaracaoVariavelInt:
    case AST_declaracaoVariavelChar:
        set_variable(node);
        node->symbol->value = set_variable_value(node->son[FIRST_SON]->symbol->text);
        break;

    case AST_declaracaoVariavelFloat:
        set_variable(node);
        node->symbol->value = set_variable_value_float(node->son[FIRST_SON]->symbol->text, node->son[SECOND_SON]->symbol->text);
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

    }

    for (i = 0; i < MAX_SONS; i++)
    {
        check_and_set_declarations(node->son[i]);
    }
}

void check_undeclared()
{
    semanticErrors += hash_check_undeclared();
}

// **************************
// Gera pilha de valores
// **************************

int is_number(char *str, int index)
{
    int retorno;

    if (str[index] >= '0' && str[index] <= '9')
    {
        retorno = is_number(str, index + 1);
    }
    else if (str[index] == '\0')
        retorno = TRUE;
    else
        retorno = FALSE;

    return retorno;
}

float processa_expr_pilha(pilha_aritmetica *cabecote)
{
    return 0;
    int operacao_atual = -1;
    float val1, val2;
    if (cabecote->tipo == PILHA_OPERADOR)
    {
        operacao_atual = cabecote->operador;
        val1 = processa_expr_pilha(cabecote->expr_esq);
        val2 = processa_expr_pilha(cabecote->expr_dir);

        switch (operacao_atual)
        {
        case AST_ADD:
            return val1 + val2;
            break;
        case AST_SUB:
            return val1 - val2;
            break;
        case AST_DIV:
            return val1 / val2;
            break;
        case AST_MULT:
            return val1 * val2;
            break;
        default:
            break;
        }
    }
    else
    {
        return cabecote->valor;
    }
}

void gera_pilha(AST *node, int level, pilha_aritmetica *cabecote, int filho)
{
    int tipo, OPERANDO = 0;
    int primeiro = 0, segundo = 1;
    if (node == 0)
        return;

    int i;

    for (i = 0; i < level; i++)
        fprintf(stderr, "  ");

    switch (node->type)
    {
    case AST_SYMBOL:
    case AST_chamadaFuncao:
    case AST_ADD:
    case AST_SUB:
    case AST_MULT:
    case AST_DIV:
        if (filho == 0)
        {
            cabecote->expr_esq = malloc(sizeof(*(cabecote->expr_esq)));
            cabecote = cabecote->expr_esq;
            printf("Esquerda! ");
        }
        else if (filho == 1)
        {
            cabecote->expr_dir = malloc(sizeof(*(cabecote->expr_dir)));
            cabecote = cabecote->expr_dir;
            printf("Direita! ");
        }
        break;
    }


    switch (node->type)
    {
    case AST_SYMBOL:
        if (is_number(node->symbol->text, 0))
        {
            cabecote->valor = atoi(node->symbol->text);
            cabecote->tipo = PILHA_VALOR;
            printf("%f\n", cabecote->valor);
        }
        else
        {
            cabecote->valor = node->symbol->value;
            cabecote->tipo = PILHA_VALOR;

            printf("%f\n", cabecote->valor);
        }
        break;
    case AST_chamadaFuncao:
        fprintf(stderr, "AST_chamadaFuncao: ");

        cabecote->valor = 0;
        cabecote->tipo = PILHA_VALOR;

        printf("%f\n", cabecote->valor);
        return;
        break;

    case AST_ADD:
        fprintf(stderr, "AST_ADD\n");
        cabecote->tipo = PILHA_OPERADOR;
        cabecote->operador = AST_ADD;
        break;
    case AST_SUB:
        fprintf(stderr, "AST_SUB\n");
        cabecote->tipo = PILHA_OPERADOR;
        cabecote->operador = AST_SUB;
        break;
    case AST_MULT:
        fprintf(stderr, "AST_MULT\n");
        cabecote->tipo = PILHA_OPERADOR;
        cabecote->operador = AST_MULT;
        break;
    case AST_DIV:
        fprintf(stderr, "AST_DIV\n");
        cabecote->tipo = PILHA_OPERADOR;
        cabecote->operador = AST_DIV;
        break;

    default:
        primeiro = 1;
        segundo = 0;
        printf("\n");
    }

    gera_pilha(node->son[0], level + 1, cabecote, primeiro);
    gera_pilha(node->son[1], level + 2, cabecote, segundo);
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

    // Vetores
    case AST_VECTOR:
        if (
            (node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_LIT_CHAR) ||
            (node->symbol->type == SYMBOL_VECTOR &&
             (node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR)))
            return TRUE;

        if ((node->symbol->type == SYMBOL_LIT_FLOAT) ||
            (node->symbol->type == SYMBOL_VECTOR && node->symbol->datatype == DATATYPE_FLOAT))
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

        counter = check_chamada_parametros_entrada(node->son[RIGHT_SON], num_param_expect, counter);
    }
    else
    {
        if (!is_exprAritmetica(node, accept_bool, accept_float, "FUNCTION PARAMETERS"))

        {
            fprintf(stderr, "Semantic ERROR: invalid operand for FUNCTION PARAMETERS\n");
            semanticErrors++;
        }
    }

    return counter;
}

void expr_check_function_call(AST *node, int accept_float, char *function_call_name)
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
              ((node->symbol->datatype == DATATYPE_INT || (node->symbol->datatype == DATATYPE_FLOAT && accept_float) ||
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

void expr_check_variable_literal_call(AST *node, int accept_float, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0;
    int error = FALSE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_SYMBOL:

        if (!(node->symbol->type == SYMBOL_LIT_CHAR || (node->symbol->type == SYMBOL_LIT_FLOAT && accept_float) ||
              node->symbol->type == SYMBOL_LIT_INT || node->symbol->type == SYMBOL_VARIABLE))
        {
            fprintf(stderr, "Semantic ERROR: invalid \'%s\' for SYMBOL\n", node->symbol->text);
            semanticErrors++;
        }
        else if (node->symbol->type == SYMBOL_VARIABLE &&
                 !(node->symbol->datatype == DATATYPE_INT || node->symbol->datatype == DATATYPE_CHAR || (node->symbol->datatype == DATATYPE_FLOAT && accept_float)))
        {
            fprintf(stderr, "Semantic ERROR: invalid variable '%s\' TYPE not accepted at %s\n", node->symbol->text, function_call_name);
            semanticErrors++;
        }

        break;

    default:
        break;
    }
}

int is_vector(HASH_NODE *node, int accept_float)
{
    switch (node->type)
    {
    case SYMBOL_VECTOR:
        switch (node->datatype)
        {
        case DATATYPE_INT:
        case DATATYPE_CHAR:
            return TRUE;
        case DATATYPE_FLOAT:
            return accept_float;
        default:
            break;
        }

    default:
        return FALSE;
    }
}

int expr_check_vector_call(AST *node, int accept_float, char *function_call_name)
{
    int INDEX = 0, VALUE = 1;
    int accept_bool = FALSE, accept_float_interno = FALSE;
    if (node == 0)
        return FALSE;

    if (node->type != AST_VECTOR)
        return FALSE;

    // Identifier
    if (!is_vector(node->symbol, accept_float))
    {
        fprintf(stderr, "Semantic ERROR: identifier \'%s\' not accepted at %s\n", node->symbol->text, function_call_name);
        semanticErrors += 1;
    }

    // Index
    !is_exprAritmetica(node->son[INDEX], accept_bool, !accept_float, "VECTOR INDEX");
    check_arithmetic_expression(node->son[INDEX], FALSE, accept_float_interno, "VECTOR INDEX");
    return TRUE;
}

void check_arithmetic_expression(AST *node, int accept_bool, int accept_float, char *function_call_name)
{
    int i;
    int LEFT_OPERAND = 0, RIGHT_OPERAND = 1;

    if (node == 0)
        return;

    expr_check_operands(node, accept_bool, accept_float, function_call_name);
    expr_check_parenteses(node, accept_bool, accept_float, function_call_name);
    expr_check_function_call(node, accept_float, function_call_name);
    expr_check_variable_literal_call(node, accept_float, function_call_name);
    int have_vector = FALSE;

    have_vector = expr_check_vector_call(node, accept_float, function_call_name);

    for (i = 0; i < MAX_SONS; i++)
    {
        if (have_vector && i == 0)
            continue;

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
    }


    check_arithmetic_expression(node->son[FIRST_OPERAND], accept_bool, accept_float, "VARIABLE VALUE");
}

void atrib_vector(AST *node)
{
    int INDEX = 0, VALUE = 1;
    int accept_bool = FALSE, accept_float = TRUE;

    // Identifier
    if (!is_vector(node->symbol, accept_float))
    {
        fprintf(stderr, "Semantic ERROR: invalid identifier \'%s\' for VECTOR ASSEGMENT\n", node->symbol->text);
        semanticErrors++;
    }

    // Index

    check_arithmetic_expression(node->son[INDEX], accept_bool, !accept_float, "VECTOR INDEX");

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
// While
// **********************

void check_while(AST *node)
{
    int i, FIRST_SON = 0, SECOND_SON = 1;
    int accept_bool = TRUE, accept_float = TRUE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_while:
        check_arithmetic_expression(node->son[FIRST_SON], accept_bool, accept_float, "WHILE");
        check_comando(node->son[SECOND_SON]);

    default:
        break;
    }
}

// **********************
// If
// **********************

void check_if(AST *node)
{
    int i, FIRST_SON = 0, SECOND_SON = 1, THIRD_SON = 2;
    int accept_bool = TRUE, accept_float = TRUE;

    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_if:
        check_arithmetic_expression(node->son[FIRST_SON], accept_bool, accept_float, "IF");
        check_comando(node->son[SECOND_SON]);
    case AST_ifElse:
        check_arithmetic_expression(node->son[FIRST_SON], accept_bool, accept_float, "IF ELSE");
        check_comando(node->son[SECOND_SON]);
        check_comando(node->son[THIRD_SON]);

    default:
        break;
    }
}

// **********************
// Comando Simples
// **********************

void check_comando_simples(AST *node)
{
    if (node == 0)
        return;

    check_atrib(node);
    check_goto(node);
    check_while(node);
    check_if(node);
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
    if (node == 0)
        return;
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
        check_decl(node->son[SECOND_SON]);
        break;

    default:
        break;
    }
}

// _____________________________________________________ //

void check_program(AST *node)
{

    pilha = malloc(sizeof(*pilha));
    check_and_set_declarations(node);
    check_decl(node);

    printf("Semantic errors: %d\n", getSemanticErrors());
}

int getSemanticErrors()
{
    return semanticErrors;
}
