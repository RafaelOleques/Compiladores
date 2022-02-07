//Autor: Rafael Oleques Nunes

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
    }
}

int main(int argc, char *argv[])
{
    int token;

    openFile(argc, argv);


    initMe();

    while (isRunning())
    {
        token = yylex();

        //Evitar pegar a marca de final de arquivo
        if (isRunning())
            switch (token)
            {
            case KW_CHAR:
                printf("KW_CHAR\n");
                break;
            case KW_INT:
                printf("KW_INT\n");
                break;
            case KW_FLOAT:
                printf("KW_FLOAT\n");
                break;

            case KW_IF:
                printf("KW_IF\n");
                break;
            case KW_THEN:
                printf("KW_THEN\n");
                break;
            case KW_ELSE:
                printf("KW_ELSE\n");
                break;
            case KW_WHILE:
                printf("KW_WHILE\n");
                break;
            case KW_GOTO:
                printf("KW_GOTO\n");
                break;
            case KW_READ:
                printf("KW_READ\n");
                break;
            case KW_PRINT:
                printf("KW_PRINT\n");
                break;
            case KW_RETURN:
                printf("KW_RETURN\n");
                break;

            case OPERATOR_LE:
                printf("OPERATOR_LE\n");
                break;
            case OPERATOR_GE:
                printf("OPERATOR_GE\n");
                break;
            case OPERATOR_EQ:
                printf("OPERATOR_EQ\n");
                break;
            case OPERATOR_DIF:
                printf("OPERATOR_DIF\n");
                break;

            case TK_IDENTIFIER:
                printf("TK_IDENTIFIER\n");
                break;

            case LIT_INTEGER:
                printf("LIT_INTEGER\n");
                break;
            case LIT_CHAR:
                printf("LIT_CHAR\n");
                break;
            case LIT_STRING:
                printf("LIT_STRING\n");
                break;

            case TOKEN_ERROR:
                printf("TOKEN_ERROR\n");
                break;
            default:
                printf("TOKEN_%c\n", token);
            }
    }

    printf("\n");
    printf("Main encerrada corretamente!\n");

    printf("\n");
    printf("Lines: %d\n", getLineNumber());

    printf("\nHash Table:\n");
    hashPrint();

    exit(0);
}
