#ifndef DIFF

#define DIFF(Node) differentiateSubtree(Node, newSize)

#endif

#ifndef COPY

#define COPY(Destination, Source) copySubtree(Destination, Source, newSize);

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef ADDNODE

#define ADDNODE nodeConstructor(OPERATION, {.operation = ADD})

#endif

#ifndef SUBNODE

#define SUBNODE nodeConstructor(OPERATION, {.operation = SUB})

#endif

#ifndef MULNODE

#define MULNODE nodeConstructor(OPERATION, {.operation = MUL})

#endif

#ifndef DIVNODE

#define DIVNODE nodeConstructor(OPERATION, {.operation = DIV})

#endif

#ifndef POWNODE

#define POWNODE nodeConstructor(OPERATION, {.operation = POW})

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(ADD,    "+",
{
    node *addNode = ADDNODE;

    printf("Created add node with address %p", addNode);

    if (Node->left)
        addNode->left  = DIFF(Node->left);

    if (Node->right)
        addNode->right = DIFF(Node->right);

    return addNode;
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(SUB,    "-",
{
    node *subNode = SUBNODE;

    if (Node->left)
        subNode->left  = DIFF(Node->left);

    if (Node->right)
        subNode->right = DIFF(Node->right);

    return subNode;
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(MUL,    "*",
{
    node *addNode  = ADDNODE;

    addNode->left  = MULNODE;
    addNode->right = MULNODE;

    if (Node->left)
    {
        addNode->left->left  = DIFF(Node->left);
        COPY(&(addNode->left->right),  Node->left);
    }
    
    if (Node->right)
    {
        addNode->right->left = DIFF(Node->right);
        COPY(&(addNode->right->right), Node->right);
    }

    return addNode;
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(DIV,    "/",
{
    node *subNode  = SUBNODE; // TODO Check on constant.

    subNode->left  = MULNODE;
    subNode->right = MULNODE;

    if (Node->left)
    {
        subNode->left->left  = DIFF(Node->left);
        COPY(&(subNode->left->right), Node->left);
    }

    if (Node->right)
    {
        subNode->right->left = DIFF(Node->right);
        COPY(&(subNode->right->right), Node->right);
    }

    node *divNode  = DIVNODE;
    divNode->left  = subNode;

    divNode->right = POWNODE;

    if (Node->right)
        COPY(&(divNode->right->left), Node->right);

    return divNode;
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(POW,    "^",
{
    return NULL;
})

DEFINE_OPERATION(SIN,    "sin",
{
    return NULL;
})

DEFINE_OPERATION(COS,    "cos",
{
    return NULL;
})

DEFINE_OPERATION(TG,     "tg",
{
    return NULL;
})

DEFINE_OPERATION(CTG,    "ctg",
{
    return NULL;
})

DEFINE_OPERATION(ARCSIN, "arcsin",
{
    return NULL;
})

DEFINE_OPERATION(ARCCOS, "arccos",
{
    return NULL;
})

DEFINE_OPERATION(LOG,    "log",
{
    return NULL;
})
