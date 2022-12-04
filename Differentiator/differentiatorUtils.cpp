#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void copySubtree (node **destNode, const node *srcNode)
{
    *destNode = nodeConstructor(srcNode->type, srcNode->data);

    if (srcNode->left != NULL)
        copySubtree(&((*destNode)->left),  srcNode->left);

    if (srcNode->right != NULL)
        copySubtree(&((*destNode)->right), srcNode->right);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void countSubtreeSize (node *Node, size_t *Size)
{
    if (Node->left)
        countSubtreeSize(Node->left,  Size);

    if (Node->right)
        countSubtreeSize(Node->right, Size);

    (*Size)++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void replaceSubtreeWithNode (node *Node, TYPE type, data_t data)
{
    nodeDestructor(Node->left);
    nodeDestructor(Node->right);

    Node->left  = NULL;
    Node->right = NULL;

    Node->type = type;
    Node->data = data;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static node *differentiateSubtree (const node *Node)
{
    switch(Node->type)
    {
        case OPERATION:
            #define DEFINE_OPERATION(name, string, ...) \
                if (Node->data.operation == name)       \
                    __VA_ARGS__
            
            #include "operations.h"

            #undef DEFINE_OPERATION

            break;

        case VALUE:
            return nodeConstructor(VALUE, {.value = 0});
            break;

        case VARIABLE:
            return nodeConstructor(VALUE, {.value = 1});
            break;

        default:
            PUTERROR("Unmatched type of node.");
            return NULL;
            break;
    }

    return NULL;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *differentiateTree (const tree *Tree)
{
    CHECKERROR(Tree != NULL &&
               "Can't differentiate NULL.",
               NULL);

    tree *Diff = treeConstructor;

    CHECKERROR(Diff != NULL && 
               "Can't create new tree", 
               NULL);

    node *diffRoot  = differentiateSubtree(Tree->root);

    Diff->root = diffRoot;
    countSubtreeSize(Diff->root, &(Diff->size));

    return Diff;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static double solveArithmetic (node *Node)
{
    double result = NAN;

    if (Node->left       != NULL  && Node->right       != NULL &&
        Node->left->type == VALUE && Node->right->type == VALUE)
    {
        double leftNumber  = Node->left->data.value;
        double rightNumber = Node->right->data.value;

        switch (Node->data.operation)
        {
            case ADD:
                leftNumber += rightNumber;
                break;
            
            case SUB:
                leftNumber -= rightNumber;
                break;

            case MUL:
                leftNumber *= rightNumber;
                break;

            case DIV:
                leftNumber /= rightNumber;
                break;

            default:
                break;
        }

        result = leftNumber; 
    }

    return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static double solveTrigonometry (node *Node)
{
    double result = NAN;

    if (Node->left != NULL && 
        Node->type == OPERATION)
    {
        double argument = Node->left->data.value;

        switch (Node->data.operation)
        {
            case SIN:
                result = sin(argument);
                break;

            case COS:
                result = cos(argument);
                break;

            case TG:
                result = tan(argument);
                break;

            case CTG:
                result = tan(M_PI / 2 - argument);
                break;

            case ARCSIN:
                result = asin(argument);
                break;

            case ARCCOS:
                result = acos(argument);
                break;

            default:
                break;
        }
    }

    return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void fillMissingNodes (node *Node)
{
    if (Node->type == OPERATION && 
        Node->data.operation == SUB &&
        Node->left  != NULL &&
        Node->right == NULL)
        replaceSubtreeWithNode(Node, VALUE, {.value = -1 * Node->left->data.value});
    
    else if (Node->type == OPERATION && 
             Node->data.operation == ADD &&
             Node->left  != NULL &&
             Node->right == NULL)
        replaceSubtreeWithNode(Node, VALUE, {.value = Node->left->data.value});

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void solveConstants (node *Node, size_t *Size)
{
    if (Node->left        != NULL && 
       (Node->left->left  != NULL || 
        Node->left->right != NULL))
        solveConstants(Node->left, Size);

    if (Node->right        != NULL && 
       (Node->right->left  != NULL || 
        Node->right->right != NULL))
        solveConstants(Node->right, Size);

    fillMissingNodes(Node);
    
    double result = solveArithmetic(Node);
    if (!isfinite(result))
        result = solveTrigonometry(Node);

    if (isfinite(result))
    {
        *Size -= 2; // Because two leafs are deleted.

        replaceSubtreeWithNode(Node, VALUE, {.value = result});
    }

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define TRANSPORTNODE(newNode)          \
    *Size -= 2;                         \
    node *pointerToNode = newNode;      \
    Node->data  = pointerToNode->data;  \
    Node->type  = pointerToNode->type;  \
    Node->left  = pointerToNode->left;  \
    Node->right = pointerToNode->right; \
    free(pointerToNode);

static ISERROR removeUnusedMuls (node *Node, size_t *Size)
{
    if (Node->left != NULL)
        removeUnusedMuls(Node->left,  Size);

    if (Node->right != NULL)
        removeUnusedMuls(Node->right, Size);

    if (Node->type == OPERATION &&
        Node->data.operation == MUL)
    {
        double leftNumber  = NAN;
        double rightNumber = NAN;

        if (Node->left->type  == VALUE)
            leftNumber  = Node->left->data.value;

        if (Node->right->type == VALUE)
            rightNumber = Node->right->data.value;

        if ((isfinite(leftNumber)  && differenceSign(leftNumber,  0) == 0) ||
            (isfinite(rightNumber) && differenceSign(rightNumber, 0) == 0))
        {
            size_t subtreeSize = 0;
            countSubtreeSize(Node, &subtreeSize);
            
            subtreeSize--;
            *Size -= subtreeSize;

            replaceSubtreeWithNode(Node, VALUE, {.value = 0});
        }

        else if (isfinite(leftNumber) && differenceSign(leftNumber, 1) == 0)
        {
            TRANSPORTNODE(Node->right);
        }

        else if (isfinite(rightNumber) && differenceSign(rightNumber, 1) == 0)
        {
            TRANSPORTNODE(Node->left);
        }
    }
    
    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR simplifyTree (tree *Tree)
{
    CHECKERROR(Tree != NULL &&
               "Can't simplify null.",
               NULLPOINTER);

    removeUnusedMuls(Tree->root, &(Tree->size));
    solveConstants(Tree->root,   &(Tree->size));

    Tree->size = 0;
    countSubtreeSize(Tree->root, &(Tree->size));

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
