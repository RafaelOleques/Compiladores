// Autor: Rafael Oleques Nunes

#include "hash.h"
#include <string.h>

HASH_NODE *table[HASH_SIZE];

void hashInit(void)
{
    int i;
    for (i = 0; i < HASH_SIZE; i++)
        table[i] = 0;
}

int hashAddres(char *text)
{
    int addres = 1, i;

    for (i = 0; i < strlen(text); i++)
        addres = (addres * text[i]) % HASH_SIZE + 1;

    return addres - 1;
}

HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int addres = hashAddres(text);

    for (node = table[addres]; node; node = node->next)
        if (strcmp(node->text, text) == 0)
            return node;

    return 0;
}

HASH_NODE *hashInsert(char *text, int type)
{
    HASH_NODE *newnode;
    int addres = hashAddres(text);

    newnode = hashFind(text);

    if (newnode != 0)
        return newnode;

    newnode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
    newnode->type = type;
    newnode->text = (char *)calloc(strlen(text) + 1, sizeof(char));

    strcpy(newnode->text, text);

    table[addres] = newnode;

    return newnode;
}

void print_function_parameters(FUNCT_PARAMETERS *parameters)
{
    if (parameters == NULL)
        return;

    fprintf(stderr, "\t%s: ", parameters->text);

    switch (parameters->datatype)
    {
    case DATATYPE_CHAR:
        fprintf(stderr, "CHAR\n");
        break;
    case DATATYPE_FLOAT:
        fprintf(stderr, "FLOAT\n");
        break;
    case DATATYPE_INT:
        fprintf(stderr, "INT\n");
        break;

    default:
        fprintf(stderr, "TYPE ERROR!\n");
        break;
    }

    print_function_parameters(parameters->next);
}

void hashPrint(void)
{
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SIZE; i++)
        for (node = table[i]; node; node = node->next)
        {
            fprintf(stderr, "Table[%d] has %s with type ", i, node->text);

            switch (node->type)
            {
            case SYMBOL_IDENTIFIER:
                fprintf(stderr, "SYMBOL_IDENTIFIER");
                break;
            case SYMBOL_LIT_INT:
                fprintf(stderr, "SYMBOL_LIT_INT");
                break;
            case SYMBOL_LIT_STRING:
                fprintf(stderr, "SYMBOL_LIT_STRING");
                break;
            case SYMBOL_LIT_CHAR:
                fprintf(stderr, "SYMBOL_LIT_CHAR");
                break;
            case SYMBOL_TRUE:
                fprintf(stderr, "SYMBOL_TRUE");
                break;
            case SYMBOL_FALSE:
                fprintf(stderr, "SYMBOL_FALSE");
                break;
            case SYMBOL_VARIABLE:
                fprintf(stderr, "SYMBOL_VARIABLE and value: %d", node->value1);
                if(node->value2)
                    fprintf(stderr, "/%d", node->value2);
                break;
            case SYMBOL_FUNCTION:
                fprintf(stderr, "SYMBOL_FUNCTION ");
                switch (node->datatype)
                {
                case DATATYPE_CHAR:
                    fprintf(stderr, "CHAR");
                    break;
                case DATATYPE_FLOAT:
                    fprintf(stderr, "FLOAT");
                    break;
                case DATATYPE_INT:
                    fprintf(stderr, "INT");
                    break;
                default:
                    break;
                }

                if (node->parameters)
                {
                    fprintf(stderr, " and parameters:\n");
                    print_function_parameters(node->parameters);
                }

                break;
            case SYMBOL_VECTOR:
                fprintf(stderr, "SYMBOL_VECTOR");
                break;
            case SYMBOL_LIT_FLOAT:
                fprintf(stderr, "SYMBOL_LIT_FLOAT");
                break;
            default:
                fprintf(stderr, "%d", node->type);
            }

            if (node->type != SYMBOL_FUNCTION)
                switch (node->datatype)
                {
                case DATATYPE_CHAR:
                    fprintf(stderr, " CHAR");
                    break;
                case DATATYPE_FLOAT:
                    fprintf(stderr, " FLOAT");
                    break;
                case DATATYPE_INT:
                    fprintf(stderr, " INT");
                    break;
                }

            fprintf(stderr, "\n");
        }
}

int hash_check_undeclared()
{

    int i, undedeclared = 0;
    HASH_NODE *node;

    for (i = 0; i < HASH_SIZE; i++)
        for (node = table[i]; node; node = node->next)
        {
            if (node->type == SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR: identifier %s undeclared\n", node->text);
                undedeclared++;
            }
        }

    return undedeclared;
}