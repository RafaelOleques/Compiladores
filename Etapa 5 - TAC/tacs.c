#include "tacs.h"

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2)
{
    TAC *newtac = 0;

    newtac = (TAC *)calloc(1, sizeof(TAC));
    newtac->type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    newtac->next = 0;

    return newtac;
}

void tacPrint(TAC *tac)
{
    if (!tac)
        return;

    if (tac->type == TAC_SYMBOL || tac->type == TAC_declaracaoVariavelFloatLiteral ||
        tac->type == TAC_declaracaoVariavelGeral || tac->type == TAC_tamVetor)
        return;

    fprintf(stderr, "TAC(");

    switch (tac->type)
    {
    /*case TAC_SYMBOL:
        fprintf(stderr, "TAC_SYMBOL");
        break;*/
    case TAC_ADD:
        fprintf(stderr, "TAC_ADD");
        break;
    case TAC_SUB:
        fprintf(stderr, "TAC_SUB");
        break;

    case AST_MULT:
        fprintf(stderr, "AST_MULT");
        break;

    case AST_DIV:
        fprintf(stderr, "AST_DIV");
        break;

    case AST_LESS:
        fprintf(stderr, "AST_LESS");
        break;

    case AST_GREATER:
        fprintf(stderr, "AST_GREATER");
        break;

    case AST_LE:
        fprintf(stderr, "AST_LE");
        break;

    case AST_GE:
        fprintf(stderr, "AST_GE");
        break;

    case AST_EQ:
        fprintf(stderr, "AST_EQ");
        break;

    case AST_DIF:
        fprintf(stderr, "AST_DIF");
        break;

    case TAC_atribuicao:
        fprintf(stderr, "TAC_atribuicao");
        break;

    case TAC_JFALSE:
        fprintf(stderr, "TAC_JFALSE");
        break;

    case TAC_LABEL:
        fprintf(stderr, "TAC_LABEL");
        break;

    case TAC_READ:
        fprintf(stderr, "TAC_READ");
        break;

    case TAC_declaracaoVariavelGeral:
        fprintf(stderr, "TAC_declaracaoVariavelGeral");
        break;

    case TAC_declaracaoVariavel:
        fprintf(stderr, "TAC_declaracaoVariavel");
        break;

    case TAC_declaracaoVetor:
        fprintf(stderr, "TAC_declaracaoVetor");
        break;

    case TAC_valoresVetor:
        fprintf(stderr, "TAC_valoresVetor");
        break;

    case TAC_RETURN:
        fprintf(stderr, "TAC_RETURN");
        break;

    case TAC_PRINT:
        fprintf(stderr, "TAC_PRINT");
        break;

    case TAC_PRINTELEMENTOS:
        fprintf(stderr, "TAC_PRINTELEMENTOS");
        break;

    default:
        fprintf(stderr, "TAC_UNKNOWN");
        break;
    }

    fprintf(stderr, ", %s", (tac->res) ? tac->res->text : "NULL");
    fprintf(stderr, ", %s", (tac->op1) ? tac->op1->text : "NULL");
    fprintf(stderr, ", %s", (tac->op2) ? tac->op2->text : "NULL");
    fprintf(stderr, ");\n");
}

void tacPrintBackwards(TAC *tac)
{
    if (!tac)
        return;

    tacPrintBackwards(tac->prev);
    tacPrint(tac);
}

TAC *tacJoin(TAC *l1, TAC *l2)
{
    TAC *point;

    if (!l1)
        return l2;

    if (!l2)
        return l1;

    for (point = l2; point->prev != 0; point = point->prev)
        ;

    point->prev = l1;

    return l2;
}

// CODE GENERATION

TAC *makeIf(TAC *condition, TAC *codeToExecute_ifTrue);
TAC *makeOperation(int operation, TAC *val1, TAC *val2);
TAC *makeDeclaracaoVariavelInt(int type, HASH_NODE *var, TAC *val);
TAC *makeAtribuicaoVetor(int operation, TAC *val1, TAC *val2);
TAC *makePrintElementos(TAC *val1, TAC *val2);

TAC *generateCode(AST *node)
{
    int i;
    TAC *result = 0;
    TAC *code[MAX_SONS];

    if (!node)
        return 0;

    // PROCESS CHILDREN

    for (i = 0; i < MAX_SONS; i++)
        code[i] = generateCode(node->son[i]);

    // PROCESS THIS NODE

    switch (node->type)
    {
    case AST_SYMBOL:
        result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
        break;

    // Operações
    case AST_SUB:
        result = makeOperation(TAC_SUB, code[0], code[1]);
        break;

    case AST_MULT:
        result = makeOperation(TAC_MULT, code[0], code[1]);
        break;

    case AST_DIV:
        result = makeOperation(TAC_DIV, code[0], code[1]);
        break;

    case AST_LESS:
        result = makeOperation(TAC_LESS, code[0], code[1]);
        break;

    case AST_GREATER:
        result = makeOperation(TAC_GREATER, code[0], code[1]);
        break;

    case AST_LE:
        result = makeOperation(TAC_LE, code[0], code[1]);
        break;

    case AST_GE:
        result = makeOperation(TAC_GE, code[0], code[1]);
        break;

    case AST_EQ:
        result = makeOperation(TAC_EQ, code[0], code[1]);
        break;

    case AST_DIF:
        result = makeOperation(TAC_DIF, code[0], code[1]);
        break;

    case AST_ADD:
        result = makeOperation(TAC_ADD, code[0], code[1]);
        break;

    case AST_atribuicao:
        result =
            tacJoin(code[0],
                    tacCreate(TAC_atribuicao,
                              node->symbol,               // Result
                              code[0] ? code[0]->res : 0, // Op1
                              0));                        // Op2

        break;

    case AST_if:
        result = makeIf(code[0], code[1]);
        break;

    case AST_READ:
        result = tacCreate(TAC_READ, makerReadValue(), 0, 0);
        break;

    case AST_declaracaoVariavel:
        result = tacJoin(code[0],
                         tacCreate(TAC_declaracaoVariavelGeral, code[0] ? code[0]->res : 0, 0, 0));
        break;

    case AST_declaracaoVariavelInt:
    case AST_declaracaoVariavelChar:
        result = tacJoin(tacJoin(code[0], code[1]),
                         tacCreate(TAC_declaracaoVariavel, node->symbol, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0));
        break;

    case AST_declaracaoVariavelFloat:
        result = tacJoin(tacJoin(code[0], code[1]),
                         tacCreate(TAC_declaracaoVariavel, node->symbol, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0));
        break;

    case AST_declaracaoVariavelFloatLiteral:
        result = tacCreate(TAC_declaracaoVariavelFloatLiteral, node->symbol, 0, 0);
        break;

    case AST_declaracaoVetorInt:
    case AST_declaracaoVetorChar:
    case AST_declaracaoVetorFloat:
        result = tacJoin(code[0],
                         tacCreate(TAC_declaracaoVetor, node->symbol, // Name
                                   code[0] ? code[0]->res : 0,        // Length
                                   0));                               // List of values
        break;

    case AST_tamVetor:
        result = tacCreate(TAC_tamVetor, node->symbol, 0, 0);
        break;

    case AST_declaracaoVetorIntValores:
    case AST_declaracaoVetorCharValores:
    case AST_declaracaoVetorFloatValores:
        result =
            tacJoin(tacJoin(code[0], code[1]),
                    tacCreate(TAC_declaracaoVetor, node->symbol, // Name
                              code[0] ? code[0]->res : 0,        // Length
                              code[1] ? code[1]->res : 0));      // List of values

        break;

    case AST_valoresVetorMultiplo:
        result = makeAtribuicaoVetor(TAC_valoresVetor, code[0], code[1]);
        break;

    case AST_return:
        result = tacJoin(code[0],
                         tacCreate(TAC_RETURN, code[0] ? code[0]->res : 0, 0, 0));

        break;

    case AST_printElementos:
        result = makePrintElementos(code[0], code[1]);
        break;

    case AST_print:
        result = tacJoin(code[0],
                         tacCreate(TAC_PRINT, code[0] ? code[0]->res : 0, 0, 0));
        break;

    default:
        // return the union of code for all childrens (subtrees)
        result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
        break;
    }

    return result;
}

// MAKE (TACS)

TAC *makeDeclaracaoVariavelInt(int type, HASH_NODE *var, TAC *val)
{
    return tacCreate(type, var, val ? val->res : 0, 0);
}

TAC *makeOperation(int operation, TAC *val1, TAC *val2)
{
    return tacJoin(tacJoin(val1, val2),
                   tacCreate(operation,
                             makeTemp(),             // Result
                             val1 ? val1->res : 0,   // Op1
                             val2 ? val2->res : 0)); // Op2
}

TAC *makePrintElementos(TAC *val1, TAC *val2)
{
    return tacJoin(tacJoin(val1, val2),
                   tacCreate(TAC_PRINTELEMENTOS,
                             makeTemp(),             // Result
                             val1 ? val1->res : 0,   // Op1
                             val2 ? val2->res : 0)); // Op2
}

TAC *makeAtribuicaoVetor(int operation, TAC *val1, TAC *val2)
{
    return tacJoin(tacJoin(val1, val2),
                   tacCreate(operation,
                             makeTemp(),             // Result
                             val1 ? val1->res : 0,   // Op1
                             val2 ? val2->res : 0)); // Op2
}

TAC *makeIf(TAC *condition, TAC *codeToExecute_ifTrue)
{
    TAC *jumpTac = 0;
    TAC *labelTac = 0;
    HASH_NODE *newLabel = 0;

    newLabel = makeLabel();

    jumpTac = tacCreate(TAC_JFALSE, newLabel, condition ? condition->res : 0, 0);
    jumpTac->prev = condition;
    labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
    labelTac->prev = codeToExecute_ifTrue;

    return tacJoin(jumpTac, labelTac);
}