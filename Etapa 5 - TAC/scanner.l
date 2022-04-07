%{
    //Autor: Rafael Oleques Nunes

    #include "hash.h"
    #include "ast.h"
    #include "parser.tab.h"

    #define TRUE 1
    #define FALSE 0

    int running;
    int countLines;
%}

%x COMMENT

%%
"\\*"                                         {BEGIN(COMMENT);}

<COMMENT>\*\\                                 {BEGIN(INITIAL);}
<COMMENT>.                                    {BEGIN(COMMENT);}


<*>\n                                       {countLines++;}

\\\\[^(\n|*)]*                      


char                                        {return KW_CHAR;}
int                                         {return KW_INT;}
float                                       {return KW_FLOAT;}

if                                          {return KW_IF;}
then                                        {return KW_THEN;}
else                                        {return KW_ELSE;}
while                                       {return KW_WHILE;}
goto                                        {return KW_GOTO;}
read                                        {return KW_READ;}
print                                       {return KW_PRINT;}
return                                      {return KW_RETURN;}

[,;:()\[\]\{\}\+\-\*\/<>=]                  {return yytext[0];}

"<="                                        {return OPERATOR_LE;}
">="                                        {return OPERATOR_GE;}
"=="                                        {return OPERATOR_EQ;}
"!="                                        {return OPERATOR_DIF;}

[-a-z_]{2,}|[a-z_]+                         {
                                                yylval.symbol = hashInsert(yytext, SYMBOL_IDENTIFIER); 
                                                return TK_IDENTIFIER;
                                            }

[0-9]+                                      {
                                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT_INT);  
                                                return LIT_INTEGER;
                                            }
\'.\'                                       {
                                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT_CHAR); 
                                                return LIT_CHAR;
                                            }
\"(\\.|[^"])+\"                             {
                                                yylval.symbol = hashInsert(yytext, SYMBOL_LIT_STRING); 
                                                return LIT_STRING;
                                            }


[ \t]

.                                           {return TOKEN_ERROR;}

%%

void initMe(void)
{
    running = TRUE;
    countLines = 1;
    hashInit();
}

int yywrap()
{
    running = FALSE;
    return 1;
}

int isRunning(void)
{
    return running;
}

int getLineNumber(void)
{
    return countLines;
}