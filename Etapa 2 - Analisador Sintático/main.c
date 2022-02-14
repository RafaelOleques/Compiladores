// Autor: Rafael Oleques Nunes

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

    printf("\n");
    printf("Main encerrada corretamente!\n");

    printf("\n");
    printf("Lines: %d\n", getLineNumber());

    printf("\nHash Table:\n");
    hashPrint();

    exit(0); // 0 is CODE for SUCCESS
}
