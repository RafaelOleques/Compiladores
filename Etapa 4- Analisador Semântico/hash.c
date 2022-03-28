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

void hashPrint(void)
{
    int i;
    HASH_NODE *node;
    for (i = 0; i < HASH_SIZE; i++)
        for (node = table[i]; node; node = node->next)
        {
            printf("Table[%d] has %s with type ", i, node->text);

            switch (node->type)
            {
            case SYMBOL_IDENTIFIER:
                printf("SYMBOL_IDENTIFIER");
                break;
            case SYMBOL_LIT_INT:
                printf("SYMBOL_LIT_INT");
                break;
            case SYMBOL_LIT_STRING:
                printf("SYMBOL_LIT_STRING");
                break;
            case SYMBOL_LIT_CHAR:
                printf("SYMBOL_LIT_CHAR");
                break;
            case SYMBOL_TRUE:
                printf("SYMBOL_TRUE");
                break;
            case SYMBOL_FALSE:
                printf("SYMBOL_FALSE");
                break;
            case SYMBOL_VARIABLE:
                printf("SYMBOL_VARIABLE");
                break;
            case SYMBOL_FUNCTION:
                printf("SYMBOL_FUNCTION");
                break;
            case SYMBOL_VECTOR:
                printf("SYMBOL_VECTOR");
                break;
            case SYMBOL_LIT_FLOAT:
                printf("SYMBOL_LIT_FLOAT");
                break;
            default:
                printf("%d", node->type);
            }

            switch (node->datatype)
            {
            case DATATYPE_CHAR:
                printf(" CHAR");
                break;
            case DATATYPE_FLOAT:
                printf(" FLOAT");
                break;
            case DATATYPE_INT:
                printf(" INT");
                break;
            }

            printf("\n");
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