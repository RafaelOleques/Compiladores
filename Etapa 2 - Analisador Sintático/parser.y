%{
    int yyerror(char*);
%}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT

%token KW_IF
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_GOTO
%token KW_READ
%token KW_PRINT
%token KW_RETURN   

%token OPERATOR_LE 
%token OPERATOR_GE 
%token OPERATOR_EQ 
%token OPERATOR_DIF

%token TK_IDENTIFIER

%token LIT_INTEGER 
%token LIT_CHAR
%token LIT_STRING  


%token TOKEN_ERROR

%%
programa:
    decl 
    ;

decl: 
    declaracaoFuncao decl
    | declaracaoVariavel ';' decl
    |
    ;

/*
    Variáveis
*/

declaracaoVariavel:
    KW_INT TK_IDENTIFIER ':' inicializacao
    | KW_CHAR TK_IDENTIFIER ':' inicializacao
    | KW_FLOAT TK_IDENTIFIER ':' LIT_INTEGER '/' LIT_INTEGER 
    | declaracaoVetor
    ;

declaracaoVetor:
    KW_INT TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor
    | KW_CHAR TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor
    | KW_INT TK_IDENTIFIER '[' LIT_INTEGER ']'
    | KW_CHAR TK_IDENTIFIER '[' LIT_INTEGER ']'
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']'
    ;

valoresVetor:
    inicializacao
    | inicializacao valoresVetor
    ;

inicializacao:
    LIT_INTEGER
    | LIT_CHAR;

/*
    Funções
*/

declaracaoFuncao:
    cabecalhoFuncao comando
    ;

cabecalhoFuncao:
    KW_INT TK_IDENTIFIER '(' parametrosEntrada ')'
    | KW_CHAR TK_IDENTIFIER '(' parametrosEntrada ')'
    | KW_FLOAT TK_IDENTIFIER '(' parametrosEntrada ')'
    ;

parametrosEntrada:
    parametroEntrada
    |
    ;

parametroEntrada:
    KW_INT TK_IDENTIFIER ',' parametroEntrada
    | KW_CHAR TK_IDENTIFIER ',' parametroEntrada
    | KW_INT TK_IDENTIFIER
    | KW_CHAR TK_IDENTIFIER
    | KW_FLOAT TK_IDENTIFIER ',' parametroEntrada
    | KW_FLOAT TK_IDENTIFIER
    ;

/*
    Comandos
*/

bloco:
    '{' comandoBloco '}'
    ;

comandoBloco:
    comando ';' comandoBloco
    | comando ';'
    | label comandoBloco
    | label
    |
    ;

comando:
    comandoSimples
    | bloco
    ;

comandoSimples:
    atribuicao
    | controleFluxo
    | print
    | return
    | 
    ;

atribuicao:
    TK_IDENTIFIER '=' exprAritmetica
    | TK_IDENTIFIER '[' exprAritmetica ']' '=' exprAritmetica
    ;

/*
    Print
*/

print:
    KW_PRINT printElementos
    ;

printElementos:
    printElemento ',' printElementos
    | printElemento
    ;

printElemento:
    LIT_STRING
    | exprAritmetica
    ;

/*
    Return
*/

return:
    KW_RETURN exprAritmetica

/*
    Expressao Aritmetica
*/
exprAritmetica:
    expressaoFolha
    | chamadaFuncao
    | KW_READ
    | '(' exprAritmetica ')'
    | exprAritmetica operador exprAritmetica
    ;    

chamadaFuncao:
    TK_IDENTIFIER '(' chamadaParametrosEntrada ')'
    ;

chamadaParametrosEntrada:
    chamadaParametroEntrada
    |
    ;

chamadaParametroEntrada:
    exprAritmetica ',' chamadaParametroEntrada
    | exprAritmetica
    ;

operador:
    '+'
    | '-'
    | '*'
    | '/'
    | '<'
    | '>'
    | OPERATOR_LE
    | OPERATOR_GE
    | OPERATOR_EQ
    | OPERATOR_DIF
    ;

expressaoFolha:
    TK_IDENTIFIER
    | TK_IDENTIFIER '[' exprAritmetica ']'
    | LIT_INTEGER
    | LIT_CHAR
    ;

label:
    TK_IDENTIFIER ':'
    ;

/*
    Fluxo
*/

controleFluxo:
    if
    | while
    | goto
    ;

goto:
    KW_GOTO TK_IDENTIFIER
    ;

if:
    KW_IF exprAritmetica KW_THEN comando
    | KW_IF exprAritmetica KW_THEN comando KW_ELSE comando
    ;

while:
    KW_WHILE exprAritmetica comando
    ;

%%

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_FLOAT 3

#define SYMBOL_IDENTIFIER 7

int yyerror(char *s)
{
    fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
    exit(3);
}