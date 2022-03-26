%{
    #include "ast.h"
    #include "hash.h"
    
    int yyerror(char*);
    int getLineNumber();
    int yylex();

    AST * program_ast;
%}

%union
{
    HASH_NODE *symbol;
    struct astnode *ast;
}

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

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER 
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING  


%token TOKEN_ERROR

%type<ast> exprAritmetica
%type<ast> expressaoFolha
%type<ast> chamadaFuncao 
%type<ast> chamadaParametrosEntrada
%type<ast> chamadaParametroEntrada
%type<ast> atribuicao
%type<ast> print
%type<ast> printElementos
%type<ast> printElemento
%type<ast> return
%type<ast> goto
%type<ast> comandoBloco
%type<ast> comando
%type<ast> label
%type<ast> bloco
%type<ast> if
%type<ast> while
%type<ast> parametrosEntrada
%type<ast> parametroEntrada
%type<ast> cabecalhoFuncao
%type<ast> declaracaoFuncao
%type<ast> inicializacao
%type<ast> valoresVetor
%type<ast> declaracaoVetor
%type<ast> declaracaoVariavel
%type<ast> decl
%type<ast> programa
%type<ast> comandoSimples
%type<ast> controleFluxo

%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'

%%
programa:
    decl                                           {
                                                        program_ast = $$;
                                                        //astPrint(program_ast, 0);
                                                        //check_and_set_declarations(program_ast);
                                                    } 
    ;

decl: 
    declaracaoFuncao decl                          {$$ = astCreate(AST_decl, 0, $1, $2, 0, 0);}  
    | declaracaoVariavel ';' decl                  {$$ = astCreate(AST_declPV, 0, $1, $3, 0, 0);} 
    |                                              {$$ = 0;}
    ;

/*
    Variáveis
*/

declaracaoVariavel:
    KW_INT TK_IDENTIFIER ':' inicializacao                      {
                                                                    $$ = astCreate(
                                                                    AST_declaracaoVariavel, 
                                                                    0, 
                                                                    astCreate(AST_declaracaoVariavelInt, $2, $4, 0, 0, 0), 
                                                                    0, 0, 0);
                                                                }
    | KW_CHAR TK_IDENTIFIER ':' inicializacao                   {
                                                                    $$ = astCreate(
                                                                    AST_declaracaoVariavel, 
                                                                    0, 
                                                                    astCreate(AST_declaracaoVariavelChar, $2, $4, 0, 0, 0), 
                                                                    0, 0, 0);
                                                                }
    | KW_FLOAT TK_IDENTIFIER ':' LIT_INTEGER '/' LIT_INTEGER    {
                                                                    $$ = astCreate(
                                                                    AST_declaracaoVariavel, 
                                                                    0, 
                                                                    astCreate(
                                                                        AST_declaracaoVariavelFloat, 
                                                                        $2, 
                                                                        astCreate(AST_declaracaoVariavelFloatLiteral, $4, 0, 0, 0, 0), 
                                                                        astCreate(AST_declaracaoVariavelFloatLiteral, $6, 0, 0, 0, 0),
                                                                         0, 0), 
                                                                    0, 0, 0);
                                                                }
    | declaracaoVetor                                           {$$ = astCreate(AST_declaracaoVariavel, 0, $1, 0, 0, 0);}
    ;

declaracaoVetor:
    KW_INT TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor       {$$ = astCreate(AST_declaracaoVetorIntValores, $2, astCreate(AST_tamVetor, $4, 0, 0, 0, 0), $7, 0, 0);}
    | KW_INT TK_IDENTIFIER '[' LIT_INTEGER ']'                      {$$ = astCreate(AST_declaracaoVetorInt, $2, astCreate(AST_tamVetor, $4, 0, 0, 0, 0), 0, 0, 0);}
    | KW_CHAR TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor    {$$ = astCreate(AST_declaracaoVetorCharValores, $2, astCreate(AST_tamVetor, $4, 0, 0, 0, 0), $7, 0, 0);}
    | KW_CHAR TK_IDENTIFIER '[' LIT_INTEGER ']'                     {$$ = astCreate(AST_declaracaoVetorChar, $2, astCreate(AST_tamVetor, $4, 0, 0, 0, 0), 0, 0, 0);}
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']' ':' valoresVetor   {$$ = astCreate(AST_declaracaoVetorFloatValores, $2, astCreate(AST_tamVetor, $4, 0, 0, 0, 0), $7, 0, 0);}
    | KW_FLOAT TK_IDENTIFIER '[' LIT_INTEGER ']'                    {$$ = astCreate(AST_declaracaoVetorFloat, $2, astCreate(AST_tamVetor, $4, 0, 0, 0, 0), 0, 0, 0);}
    ;

valoresVetor:
    inicializacao                   {$$ = astCreate(AST_valoresVetor, 0, $1, 0, 0, 0);}
    | inicializacao valoresVetor    {$$ = astCreate(AST_valoresVetorMultiplo, 0, $1, $2, 0, 0);}
    ;

inicializacao:
    LIT_INTEGER     {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    | LIT_CHAR     {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    ;

/*
    Funções
*/

declaracaoFuncao:
    cabecalhoFuncao comando {$$ = astCreate(AST_declaracaoFuncao, 0, $1, $2, 0, 0);}
    ;

cabecalhoFuncao:
    KW_INT TK_IDENTIFIER '(' parametrosEntrada ')'      {$$ = astCreate(AST_cabecalhoFuncaoInt, $2, $4, 0, 0, 0);}
    | KW_CHAR TK_IDENTIFIER '(' parametrosEntrada ')'   {$$ = astCreate(AST_cabecalhoFuncaoChar, $2, $4, 0, 0, 0);}
    | KW_FLOAT TK_IDENTIFIER '(' parametrosEntrada ')'  {$$ = astCreate(AST_cabecalhoFuncaoFloat, $2, $4, 0, 0, 0);}
    ;

parametrosEntrada:
    parametroEntrada
    |                       {$$ = 0}
    ;

parametroEntrada:
    KW_INT TK_IDENTIFIER ',' parametroEntrada       {$$ = astCreate(AST_parametroEntradaIntMultiplo, $2, $4, 0, 0, 0);}
    | KW_CHAR TK_IDENTIFIER ',' parametroEntrada    {$$ = astCreate(AST_parametroEntradaCharMultiplo, $2, $4, 0, 0, 0);}
    | KW_INT TK_IDENTIFIER                          {$$ = astCreate(AST_parametroEntradaInt, $2, 0, 0, 0, 0);}
    | KW_CHAR TK_IDENTIFIER                         {$$ = astCreate(AST_parametroEntradaChar, $2, 0, 0, 0, 0);}
    | KW_FLOAT TK_IDENTIFIER ',' parametroEntrada   {$$ = astCreate(AST_parametroEntradaFloatMultiplo, $2, $4, 0, 0, 0);}
    | KW_FLOAT TK_IDENTIFIER                        {$$ = astCreate(AST_parametroEntradaFloat, $2, 0, 0, 0, 0);}
    ;

/*
    Comandos
*/

bloco:
    '{' comandoBloco '}'         {$$ = astCreate(AST_bloco, 0, $2, 0, 0, 0);}
    ;

comandoBloco:
    comando ';' comandoBloco    {$$ = astCreate(AST_comandoBloco, 0, $1, $3, 0, 0);}
    | comando ';'               {$$ = astCreate(AST_comandoBloco, 0, $1, 0, 0, 0);}
    | label comandoBloco        {$$ = astCreate(AST_comandoBloco_label, 0, $1, $2, 0, 0);}
    | label                     
    |                           {$$ = 0}
    ;

label:
    TK_IDENTIFIER ':'          {$$ = astCreate(AST_label, $1, 0, 0, 0, 0);}
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
    |                   {$$ = 0;}
    ;

atribuicao:
    TK_IDENTIFIER '=' exprAritmetica                            {$$ = astCreate(AST_atribuicao, $1, $3, 0, 0, 0);}
    | TK_IDENTIFIER '[' exprAritmetica ']' '=' exprAritmetica   {$$ = astCreate(AST_atribuicao_vector, $1, $3, $6, 0, 0);}
    ;

/*
    Print
*/

print:
    KW_PRINT printElementos       {$$ = astCreate(AST_print, 0, $2, 0, 0, 0);}
    ;

printElementos:
    printElemento ',' printElementos  {$$ = astCreate(AST_printElementos, 0, $1, $3, 0, 0);}
    | printElemento                   
    ;

printElemento:
    LIT_STRING                        {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
    | exprAritmetica                  {$$ =  $1;}
    ;

/*
    Return
*/

return:
    KW_RETURN exprAritmetica        {$$ = astCreate(AST_return, 0, $2, 0, 0, 0);}

/*
    Expressao Aritmetica
*/
exprAritmetica:
    expressaoFolha                                   
    | chamadaFuncao                                            
    | KW_READ                                       {$$ = astCreate(AST_READ, 0, 0, 0, 0, 0);}          
    | '(' exprAritmetica ')'                        {$$ = astCreate(AST_EXPR_PARENTESES, 0, $2, 0, 0, 0);}  
    | exprAritmetica '+' exprAritmetica             {$$ = astCreate(AST_ADD, 0, $1, $3, 0, 0);}   
    | exprAritmetica '-' exprAritmetica             {$$ = astCreate(AST_SUB, 0, $1, $3, 0, 0);}     
    | exprAritmetica '*' exprAritmetica             {$$ = astCreate(AST_MULT, 0, $1, $3, 0, 0);}      
    | exprAritmetica '/' exprAritmetica             {$$ = astCreate(AST_DIV, 0, $1, $3, 0, 0);}     
    | exprAritmetica '<' exprAritmetica             {$$ = astCreate(AST_LESS, 0, $1, $3, 0, 0);}  
    | exprAritmetica '>' exprAritmetica             {$$ = astCreate(AST_GREATER, 0, $1, $3, 0, 0);}  
    | exprAritmetica OPERATOR_LE exprAritmetica     {$$ = astCreate(AST_LE, 0, $1, $3, 0, 0);}  
    | exprAritmetica OPERATOR_GE exprAritmetica     {$$ = astCreate(AST_GE, 0, $1, $3, 0, 0);}  
    | exprAritmetica OPERATOR_EQ exprAritmetica     {$$ = astCreate(AST_EQ, 0, $1, $3, 0, 0);}  
    | exprAritmetica OPERATOR_DIF exprAritmetica    {$$ = astCreate(AST_DIF, 0, $1, $3, 0, 0);}  
    ;    

chamadaFuncao:
    TK_IDENTIFIER '(' chamadaParametrosEntrada ')'  {$$ = astCreate(AST_chamadaFuncao, $1, 0, $3, 0, 0);}
    ;

chamadaParametrosEntrada:
    chamadaParametroEntrada
    |                                               {$$ = 0;}
    ;

chamadaParametroEntrada:
    exprAritmetica ',' chamadaParametroEntrada     {$$ = astCreate(AST_chamadaParametroEntrada, 0, $1, $3, 0, 0);}
    | exprAritmetica                               
    ;


expressaoFolha:
    TK_IDENTIFIER                                   {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}        
    | TK_IDENTIFIER '[' exprAritmetica ']'          {$$ = astCreate(AST_VECTOR, $1, $3, 0, 0, 0);}  
    | LIT_INTEGER                                   {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}    
    | LIT_CHAR                                      {$$ = astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}
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
    KW_GOTO TK_IDENTIFIER       {$$ = astCreate(AST_goto, $2, 0, 0, 0, 0);}
    ;

if:
    KW_IF exprAritmetica KW_THEN comando                    {$$ = astCreate(AST_if, 0, $2, $4, 0, 0);}
    | KW_IF exprAritmetica KW_THEN comando KW_ELSE comando  {$$ = astCreate(AST_ifElse, 0, $2, $4, $6, 0);}
    ;

while:
    KW_WHILE exprAritmetica comando     {$$ = astCreate(AST_while, 0, $2, $3, 0, 0);}
    ;

%%

/*
#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_FLOAT 3

#define SYMBOL_IDENTIFIER 7
*/
int yyerror(char *s)
{
    fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
    exit(3);
}