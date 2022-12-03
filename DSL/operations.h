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

#ifndef SINNODE

#define SINNODE nodeConstructor(OPERATION, {.operation = SIN})

#endif

#ifndef COSNODE

#define COSNODE nodeConstructor(OPERATION, {.operation = COS})

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef VALNODE

#define VALNODE(Value) nodeConstructor(VALUE, {.value = Value})

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(ADD,    "+",
{
    node *addNode = ADDNODE;

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
        COPY(&(addNode->left->right),  Node->right);
    }
    
    if (Node->right)
    {
        addNode->right->left = DIFF(Node->right);
        COPY(&(addNode->right->right), Node->left);
    }

    *newSize += 2;

    return addNode;
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(DIV,    "/",
{
    node *subNode  = SUBNODE;

    subNode->left  = MULNODE;
    subNode->right = MULNODE;

    if (Node->left)
    {
        subNode->left->left  = DIFF(Node->left);
        COPY(&(subNode->left->right), Node->right);
    }

    if (Node->right)
    {
        subNode->right->left = DIFF(Node->right);
        COPY(&(subNode->right->right), Node->left);
    }

    node *divNode  = DIVNODE;
    divNode->left  = subNode;

    divNode->right = POWNODE;

    if (Node->right)
        COPY(&(divNode->right->left), Node->right);

    divNode->right->right = VALNODE(2);

    *newSize += 4;

    return divNode;
})

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_OPERATION(POW,    "^",
{
    node *mulNode  = MULNODE;
    mulNode->right = MULNODE;

    node *powNode = POWNODE;
    if (Node->left)
        COPY(&(powNode->left), Node->left);

    node *subNode = SUBNODE;
    subNode->right = VALNODE(1);
    COPY(&(subNode->left ), Node->right);

    powNode->right = subNode;

    mulNode->right->right = powNode;
    COPY(&(mulNode->right->left), Node->right)

    mulNode->left = DIFF(Node->left);

    *newSize += 4;

    return mulNode;
})

DEFINE_OPERATION(SIN,    "sin",
{
    CHECKERROR(Node->left != NULL &&
               "Can't differentiate sin without argument.", 
               NULL);

    node *mulNode = MULNODE;

    mulNode->left = DIFF(Node->left);

    mulNode->right = COSNODE;
    COPY(&(mulNode->right->left), Node->left);

    (*newSize)++;

    return mulNode;
})

DEFINE_OPERATION(COS,    "cos",
{
    CHECKERROR(Node->left != NULL &&
               "Can't differentiate cos without argument.", 
               NULL);

    node *mulNode  = MULNODE;
    mulNode->left  = VALNODE(-1);
    mulNode->right = MULNODE;

    mulNode->right->left = DIFF(Node->left);

    mulNode->right->right = SINNODE;
    COPY(&(mulNode->right->right->left), Node->left);

    *newSize += 3;

    return mulNode;
})

DEFINE_OPERATION(TG,     "tg",
{
    CHECKERROR(Node->left != NULL &&
               "Can't differentiate tg without argument.", 
               NULL);

    node *divNode = DIVNODE;

    divNode->left = DIFF(Node->left);

    divNode->right = POWNODE;
    divNode->right->left  = COSNODE;
    divNode->right->right = VALNODE(2);
    
    COPY(&(divNode->right->left->left), Node->left);

    *newSize += 3;

    return divNode;
})

DEFINE_OPERATION(CTG,    "ctg",
{
    CHECKERROR(Node->left != NULL &&
               "Can't differentiate ctg without argument.", 
               NULL);

    node *mulNode = MULNODE;
    node *divNode = DIVNODE;

    divNode->left = DIFF(Node->left);

    divNode->right = POWNODE;
    divNode->right->left  = SINNODE;
    divNode->right->right = VALNODE(2);
    
    COPY(&(divNode->right->left->left), Node->left);

    mulNode->left  = VALNODE(-1);
    mulNode->right = divNode;

    *newSize += 5;

    return mulNode;
})

DEFINE_OPERATION(ARCSIN, "arcsin",
{
    node *divNode = DIVNODE;
    divNode->left = DIFF(Node->left);

    divNode->right = POWNODE;
    divNode->right->right = VALNODE(0.5);

    node *subNode  = SUBNODE;
    subNode->right = POWNODE;
    subNode->left  = VALNODE(1);

    subNode->right->right = VALNODE(2);
    COPY(&(subNode->right->left), Node->left);

    divNode->right->left = subNode;

    *newSize += 6;
    
    return divNode;
})

DEFINE_OPERATION(ARCCOS, "arccos",
{
    CHECKERROR(Node->left != NULL &&
               "Can't differentiate arccos without argument.", 
               NULL);

    node *mulNode = MULNODE;
    mulNode->left = VALNODE(-1);

    node *divNode = DIVNODE;
    divNode->left = DIFF(Node->left);

    divNode->right = POWNODE;
    divNode->right->right = VALNODE(0.5);

    node *subNode  = SUBNODE;
    subNode->right = POWNODE;
    subNode->left  = VALNODE(1);

    subNode->right->right = VALNODE(2);
    COPY(&(subNode->right->left), Node->left);

    divNode->right->left = subNode;

    mulNode->right = divNode;

    *newSize += 8;

    return mulNode;
})
