//Autor: Rafael Oleques Nunes

#ifndef HASH_HEADER
#define HASH_HEADER

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#define HASH_SIZE 997

#define SYMBOL_IDENTIFIER 1
#define SYMBOL_LIT_INT 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_LIT_CHAR 4
#define SYMBOL_TRUE 5
#define SYMBOL_FALSE 6
#define SYMBOL_VARIABLE 7
#define SYMBOL_FUNCTION 8
#define SYMBOL_VECTOR 9
#define SYMBOL_LIT_FLOAT 10
#define SYMBOL_LABLE 11

#define SYMBOL_LABLE_JFALSE 12

#define DATATYPE_INVALID -1
#define DATATYPE_INT 1
#define DATATYPE_CHAR 2
#define DATATYPE_FLOAT 3

typedef struct funct_parameters
{
    int datatype;
    int type;
    char *text;
    struct funct_parameters *next;    
} FUNCT_PARAMETERS;

typedef struct hash_node
{
    int type;
    int datatype;
    int datastruct;
    float value; //variáveis
    int vector_size;
    int is_context;
    FUNCT_PARAMETERS *parameters;
    char *text;
    struct hash_node *next;    
} HASH_NODE;

void hashInit(void);
int hashAddres(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
int hash_check_undeclared(void);
HASH_NODE *makeTemp();
HASH_NODE *makeLabel();
HASH_NODE *makerReadValue();

#endif

