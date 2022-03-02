#include "ast.h"
#include <string.h>

void descompila(AST *node, char *fileName);
void run_descompila(AST *node, FILE *file);

void descompila(AST *node, char *fileName)
{
    FILE *file;
    file = fopen(fileName, "w");
    if (!file)
        perror("fopen");

    if (file == NULL)
    {
        printf("Erro no arquivo de saida!");
        exit(1);
    }

    run_descompila(node, file);

    fclose(file);
}

#define ELEMENTOS 8
#define TEXTO 12

void run_descompila(AST *node, FILE *file)
{
    char completar[ELEMENTOS][TEXTO];
    int cabecote_escrita = 0, cabecote_leitura = 0;

    if (node == 0)
        return;

    int i;

    switch (node->type)
    {
    case AST_ADD:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " + ");
        break;
    case AST_SUB:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " - ");
        break;
    case AST_MULT:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " * ");
        break;
    case AST_DIV:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " / ");
        break;
    case AST_LESS:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " < ");
        break;
    case AST_GREATER:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " > ");
        break;
    case AST_LE:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " <= ");
        break;
    case AST_GE:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " >= ");
        break;
    case AST_EQ:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " == ");
        break;
    case AST_DIF:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " != ");
        break;
    case AST_chamadaFuncao:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "(");
        strcpy(completar[cabecote_escrita++], ")");
        break;
    case AST_chamadaParametroEntrada:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], ", ");
        break;
    case AST_READ:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " read");
        break;
    case AST_atribuicao:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " = ");
        break;
    case AST_atribuicao_vector:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "] = ");
        break;
    case AST_print:
        strcpy(completar[cabecote_escrita++], " print ");
        break;
    case AST_printElementos:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], ", ");
        break;
    case AST_return:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " return ");
        break;
    case AST_goto:
        strcpy(completar[cabecote_escrita++], " goto ");
        break;
    case AST_label:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], ": ");
        break;
    case AST_bloco:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "\n{\n");
        strcpy(completar[cabecote_escrita++], "}\n");
        break;
    case AST_if:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " if ");
        strcpy(completar[cabecote_escrita++], " then ");
        break;
    case AST_ifElse:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " if ");
        strcpy(completar[cabecote_escrita++], " then ");
        strcpy(completar[cabecote_escrita++], " else ");
        break;
    case AST_while:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " while ");
        break;
    case AST_comandoBloco:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], ";\n");
        break;
    case AST_VECTOR:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]");
        break;
    case AST_parametroEntradaIntMultiplo:
        strcpy(completar[cabecote_escrita++], " int ");
        strcpy(completar[cabecote_escrita++], ", ");
        break;
    case AST_parametroEntradaCharMultiplo:
        strcpy(completar[cabecote_escrita++], " char ");
        strcpy(completar[cabecote_escrita++], ", ");

        break;
    case AST_parametroEntradaFloatMultiplo:
        strcpy(completar[cabecote_escrita++], " float ");
        strcpy(completar[cabecote_escrita++], ", ");
        break;
    case AST_parametroEntradaInt:
        strcpy(completar[cabecote_escrita++], " int ");
        break;
    case AST_parametroEntradaChar:
        strcpy(completar[cabecote_escrita++], " char ");
        break;
    case AST_parametroEntradaFloat:
        strcpy(completar[cabecote_escrita++], " float ");
        break;
    case AST_cabecalhoFuncaoInt:
        strcpy(completar[cabecote_escrita++], " int ");
        strcpy(completar[cabecote_escrita++], "(");
        strcpy(completar[cabecote_escrita++], ")");
        break;
    case AST_cabecalhoFuncaoChar:
        strcpy(completar[cabecote_escrita++], " char ");
        strcpy(completar[cabecote_escrita++], "(");
        strcpy(completar[cabecote_escrita++], ")");
        break;
    case AST_cabecalhoFuncaoFloat:
        strcpy(completar[cabecote_escrita++], " float ");
        strcpy(completar[cabecote_escrita++], "(");
        strcpy(completar[cabecote_escrita++], ")");
        break;
    case AST_valoresVetorMultiplo:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], " ");
        break;
    case AST_declaracaoVetorInt:
        strcpy(completar[cabecote_escrita++], " int ");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]");
        break;
    case AST_declaracaoVetorChar:
        strcpy(completar[cabecote_escrita++], " char ");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]");
        break;
    case AST_declaracaoVetorFloat:
        strcpy(completar[cabecote_escrita++], " float ");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]");
        break;
    case AST_declaracaoVetorIntValores:
        strcpy(completar[cabecote_escrita++], " int ");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]: ");
        break;
    case AST_declaracaoVetorCharValores:
        strcpy(completar[cabecote_escrita++], " char ");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]: ");
        break;
    case AST_declaracaoVetorFloatValores:
        strcpy(completar[cabecote_escrita++], " float ");
        strcpy(completar[cabecote_escrita++], "[");
        strcpy(completar[cabecote_escrita++], "]: ");
        break;
    case AST_declaracaoVariavelInt:
        strcpy(completar[cabecote_escrita++], " int ");
        strcpy(completar[cabecote_escrita++], ":");
        break;
    case AST_declaracaoVariavelChar:
        strcpy(completar[cabecote_escrita++], "char ");
        strcpy(completar[cabecote_escrita++], ":");
        break;
    case AST_declaracaoVariavelFloat:
        strcpy(completar[cabecote_escrita++], " float ");
        strcpy(completar[cabecote_escrita++], ":");
        strcpy(completar[cabecote_escrita++], "/");
        break;
    case AST_declPV:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], ";\n");
        break;
    case AST_EXPR_PARENTESES:
        strcpy(completar[cabecote_escrita++], "");
        strcpy(completar[cabecote_escrita++], "(");
        strcpy(completar[cabecote_escrita++], ")");
        break;

    default:
        break;
    }

    if (cabecote_leitura < cabecote_escrita)
    {
        fprintf(file, "%s", completar[cabecote_leitura]);
        cabecote_leitura++;
    }

    if (node->symbol != 0)
        fprintf(file, "%s", node->symbol->text);

    for (i = 0; i < MAX_SONS; i++)
    {
        if (cabecote_leitura < cabecote_escrita)
        {
            fprintf(file, "%s", completar[cabecote_leitura]);
            cabecote_leitura++;
        }
        run_descompila(node->son[i], file);
    }

    if (cabecote_leitura < cabecote_escrita)
    {
        fprintf(file, "%s", completar[cabecote_leitura]);
        cabecote_leitura++;
    }
}