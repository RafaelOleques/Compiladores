/*

========> DÚVIDAS

1)Posso ter um make read? 
	Posso por ele como int, já que int pode ir para qualquer tipo?

2)Posso usar o mesmo tac para os diferentes tipos?
	Diferenciando o float com aquele que tem dois operandos

*/

#define AST_chamadaFuncao 12
#define AST_chamadaParametrosEntrada 13
#define AST_chamadaParametroEntrada 14
#define AST_atribuicao_vector 17
#define AST_comandoBloco_label 24
#define AST_bloco 25
#define AST_while 27
#define AST_comandoBloco 28
#define AST_VECTOR 29
#define AST_parametroEntradaInt 30
#define AST_parametroEntradaChar 31
#define AST_parametroEntradaFloat 32
#define AST_cabecalhoFuncaoInt 33
#define AST_cabecalhoFuncaoChar 34
#define AST_cabecalhoFuncaoFloat 35
#define AST_declaracaoFuncao 36
#define AST_valoresVetor 37
#define AST_decl 47
#define AST_declPV 48
#define AST_EXPR_PARENTESES 49
#define AST_ifElse 50
#define AST_parametroEntradaIntMultiplo 51
#define AST_parametroEntradaCharMultiplo 52
#define AST_parametroEntradaFloatMultiplo 53

#define AST_goto 22
#define AST_label 23

//Acho que não precisa
#define AST_printElemento 20