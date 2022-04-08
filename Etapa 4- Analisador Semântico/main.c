// Autor: Rafael Oleques Nunes

#include <stdio.h>
#include "hash.h"
#include "descompilador.c"
#include "semantic.h"

extern FILE *yyin;
int yyparser();
void initMe();
int yyparse();
int getLineNumber();
AST *program_ast;

void openFile(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Cal: .\a.exe filename\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");

    if (yyin == 0)
    {
        fprintf(stderr, "Erro ao abrir o arquivo!\n");
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int token;

    openFile(argc, argv);

    initMe();

    yyparse();

    descompila(program_ast, argv[2]);

    fprintf(stderr, "\n");
    fprintf(stderr, "Main encerrada corretamente!\n");

    fprintf(stderr, "\n");
    fprintf(stderr, "Lines: %d\n", getLineNumber());

    astPrint(program_ast, 0);

    // Semantic analyse
    
    fprintf(stderr, "\n");
    check_program(program_ast);
    fprintf(stderr, "\n");

    fprintf(stderr, "\nHash Table:\n");
    hashPrint();

    if (getSemanticErrors() > 0)
        exit(4);
    else
        exit(0); // 0 is CODE for SUCCESS
}
