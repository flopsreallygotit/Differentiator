#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void copySubtree (node **destNode, const node *srcNode, size_t *subtreeSize)
{
    (*subtreeSize)++;

    *destNode = nodeConstructor(srcNode->type, srcNode->data);

    if (srcNode->left)
        copySubtree(&((*destNode)->left),  srcNode->left,  subtreeSize);

    if (srcNode->right)
        copySubtree(&((*destNode)->right), srcNode->right, subtreeSize);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static node *differentiateSubtree (const node *Node, size_t *newSize)
{
    switch(Node->type)
    {
        case OPERATION:
            (*newSize)++;

            #define DEFINE_OPERATION(name, string, ...) \
                if (Node->data.operation == name)       \
                    __VA_ARGS__
            
            #include "operations.h"

            #undef DEFINE_OPERATION

            break;

        case VALUE:
            (*newSize)++;
            return nodeConstructor(VALUE, {.value = 0});
            break;

        case VARIABLE:
            (*newSize)++;
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

    size_t diffSize = 0;
    node *diffRoot  = differentiateSubtree(Tree->root, &diffSize);

    Diff->root = diffRoot;
    Diff->size = diffSize;

    return Diff;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR solveConstants (node *Node, size_t *Size)
{
    if (Node->left        != NULL && 
       (Node->left->left  != NULL || 
        Node->left->right != NULL))
        solveConstants(Node->left, Size);

    if (Node->right        != NULL && 
       (Node->right->left  != NULL || 
        Node->right->right != NULL))
        solveConstants(Node->right, Size);

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

    else if (Node->left != NULL && 
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

    if (isfinite(result))
    {
        *Size -= 2;
        nodeDestructor(Node->left);
        nodeDestructor(Node->right);

        Node->left  = NULL;
        Node->right = NULL;

        Node->type = VALUE;
        Node->data = {.value = result};
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR simplifyTree (tree *Tree)
{
    CHECKERROR(Tree != NULL &&
               "Can't simplify null.",
               NULLPOINTER);

    solveConstants(Tree->root, &(Tree->size));

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
