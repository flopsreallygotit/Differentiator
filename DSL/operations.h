#ifndef DIFFSUBTREES

#define DIFFSUBTREES(Node)                     \
    (*newSize)++;                              \
    differentiateSubtree(Node->left, newSize); \
    differentiateSubtree(Node->left, newSize)

#endif

DEFINE_OPERATION(ADD,    "+",
{
    node *addNode = nodeConstructor(OPERATION, {.operation = ADD});

    DIFFSUBTREES(addNode);

    return addNode;
})

DEFINE_OPERATION(SUB,    "-",
{
    node *subNode = nodeConstructor(OPERATION, {.operation = SUB});

    DIFFSUBTREES(subNode);

    return subNode;
})

DEFINE_OPERATION(MUL,    "*")

DEFINE_OPERATION(DIV,    "/",     )
DEFINE_OPERATION(POW,    "^",     )
DEFINE_OPERATION(SIN,    "sin",   )
DEFINE_OPERATION(COS,    "cos",   )
DEFINE_OPERATION(TG,     "tg",    )
DEFINE_OPERATION(CTG,    "ctg",   )
DEFINE_OPERATION(ARCSIN, "arcsin",)
DEFINE_OPERATION(ARCCOS, "arccos",)
DEFINE_OPERATION(LOG,    "log",   )