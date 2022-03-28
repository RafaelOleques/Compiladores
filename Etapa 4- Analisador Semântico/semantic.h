#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

extern int semanticErrors;

void check_and_set_declarations(AST *node);
void check_undeclared();
void check_program(AST *node);

int getSemanticErrors();

#endif
//END OF FILE