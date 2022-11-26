#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static node *differenciateSubtree (node *Node, size_t *newSize)
{
    node *newNode = NULL;

    switch(Node->type)
    {
        case OPERATION:
            
            break;

        case VALUE:
            break;

        case VARIABLE:
            break;

        default:
            PUTERROR("Unmatched type of node.");
            return NULL;
            break;
    }

    return newNode;
}

tree *differentiateTree (tree *Tree)
{
    CHECKERROR(Tree != NULL &&
               "Can't differentiate NULL.",
               NULL);

    tree *Diff = treeConstructor;

    CHECKERROR(Diff == NULL && 
               "Can't create new tree", 
               NULL);

    size_t diffSize = 0;
    node *diffRoot  = differenciateSubtree(Tree->root, &diffSize);

    Diff->root = diffRoot;
    Diff->size = diffSize;

    return Diff;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
