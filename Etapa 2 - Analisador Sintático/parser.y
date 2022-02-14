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
    tipoIntChar TK_IDENTIFIER ':' inicializacao
    | KW_FLOAT TK_IDENTIFIER ':' LIT_INTEGER '/' LIT_INTEGER 
    | declaracaoVetor
    ;

declaracaoVetor:
    tipoIntChar TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor
    | tipoIntChar TK_IDENTIFIER '[' LIT_INTEGER ']'
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
    Tipos
*/

tipoIntChar:
    KW_INT
    | KW_CHAR
    ;

/*
    Funções
*/

declaracaoFuncao:
    cabecalhoFuncao corpoFuncao
    ;

cabecalhoFuncao:
    tipoIntChar TK_IDENTIFIER '(' parametrosEntrada ')'
    | KW_FLOAT TK_IDENTIFIER '(' parametrosEntrada ')'
    ;

parametrosEntrada:
    parametroEntrada
    |
    ;

parametroEntrada:
    tipoIntChar TK_IDENTIFIER ',' parametroEntrada
    | tipoIntChar TK_IDENTIFIER
    | KW_FLOAT TK_IDENTIFIER ',' parametroEntrada
    | KW_FLOAT TK_IDENTIFIER
    ;

corpoFuncao:
    comando
    | bloco
    ;

/*
    Comandos
*/

bloco:
    '{' comandosBloco '}'
    ;

comandosBloco:
    comandoBloco
    | 
    ;

comandoBloco:
    comando ';' comandoBloco
    | comando ';'
    | label comandoBloco
    | label
    ;

comando:
    comandoSimples
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
    expressaoAritmetica
    | expressaoFolha
    | chamadaFuncao
    | KW_READ
    ;

expressaoAritmetica:
    expressaoFolha
    | '(' expressaoAritmetica ')'
    | expressaoAritmetica operador expressaoAritmetica
    ;

chamadaFuncao:
    TK_IDENTIFIER '(' chamadaParametrosEntrada ')'
    ;

chamadaParametrosEntrada:
    chamadaParametroEntrada
    |
    ;

chamadaParametroEntrada:
    parametroFuncao ',' chamadaParametroEntrada
    | parametroFuncao
    ;

parametroFuncao:
    TK_IDENTIFIER
    | LIT_CHAR
    | LIT_INTEGER
    | LIT_STRING
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
    KW_GOTO exprAritmetica
    ;

if:
    KW_IF exprAritmetica KW_THEN comandoFluxo
    | KW_IF exprAritmetica KW_THEN comandoFluxo KW_ELSE comandoFluxo
    ;

while:
    KW_WHILE exprAritmetica comandoFluxo
    ;

comandoFluxo:
    bloco
    | comandoSimples
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