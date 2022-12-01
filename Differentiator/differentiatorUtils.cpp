#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void copySubtree (node **destNode, node *srcNode, size_t *subtreeSize)
{
    (*subtreeSize)++;

    *destNode = nodeConstructor(srcNode->type, srcNode->data);

    if (srcNode->left)
        copySubtree(&((*destNode)->left),  srcNode->left,  subtreeSize);

    if (srcNode->right)
        copySubtree(&((*destNode)->right), srcNode->right, subtreeSize);

    return;
}

// static node *differentiateSubtree (node *Node, size_t *newSize)
// {
//     switch(Node->type)
//     {
//         case OPERATION:
//             (*newSize)++;

//             #define DEFINE_OPERATION(name, string, ...) \
//                 if (Node->data.operation == name)       \
//                     __VA_ARGS__
            
//             #include "operations.h"

//             #undef DEFINE_OPERATION

//             break;

//         case VALUE:
//             (*newSize)++;
//             return nodeConstructor(VALUE, {.value = 0});
//             break;

//         case VARIABLE:
//             (*newSize)++;
//             return nodeConstructor(VALUE, {.value = 1});
//             break;

//         default:
//             PUTERROR("Unmatched type of node.");
//             return NULL;
//             break;
//     }

//     return NULL;
// }

// tree *differentiateTree (tree *Tree)
// {
//     CHECKERROR(Tree != NULL &&
//                "Can't differentiate NULL.",
//                NULL);

//     tree *Diff = treeConstructor;

//     CHECKERROR(Diff == NULL && 
//                "Can't create new tree", 
//                NULL);

//     size_t diffSize = 0;
//     node *diffRoot  = differentiateSubtree(Tree->root, &diffSize);

//     Diff->root = diffRoot;
//     Diff->size = diffSize;

//     return Diff;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
