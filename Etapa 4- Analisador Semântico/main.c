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
        printf("Erro ao abrir o arquivo!\n");
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

    printf("\n");
    printf("Main encerrada corretamente!\n");

    printf("\n");
    printf("Lines: %d\n", getLineNumber());

    printf("\nHash Table:\n");

    astPrint(program_ast, 0);

    // Semantic analyse
    check_and_set_declarations(program_ast);
    check_undeclared();
    printf("\n");
    check_operands(program_ast);
    printf("\n");

    hashPrint();

    if (getSemanticErrors() > 0)
        exit(4);
    else
        exit(0); // 0 is CODE for SUCCESS
}
