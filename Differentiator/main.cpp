#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    // const char *filename = simpleCommandLineParser(argc, argv);

    // CHECKERROR(filename != NULL &&
    //            "Error in filename parsing.", 
    //            -1);

    // tree *Tree = parseFile(filename);

    tree *Tree = treeConstructor;

    insertRoot(Tree, OPERATION, {.operation = MUL});

    node *leftNode  = nodeConstructor(OPERATION, {.operation = ADD});
    insertLeafToNode(Tree, Tree->root, leftNode);

    node *rightNode = nodeConstructor(OPERATION, {.operation = ADD});
    insertLeafToNode(Tree, Tree->root, rightNode);

    node *leftLeftNode  = nodeConstructor(VARIABLE, {.variable = 'x'});
    node *leftRightNode = nodeConstructor(VALUE,    {.value    =  2 });
    insertLeafToNode(Tree, Tree->root->left, leftLeftNode);
    insertLeafToNode(Tree, Tree->root->left, leftRightNode);

    node *rightLeftNode  = nodeConstructor(VARIABLE, {.variable = 'x'});
    node *rightRightNode = nodeConstructor(VALUE,    {.value    =  3 });
    insertLeafToNode(Tree, Tree->root->right, rightLeftNode);
    insertLeafToNode(Tree, Tree->root->right, rightRightNode);

    inorderPrintTree(Tree);

    FILE *output = fopen("output.htm", "w");
    treeDump(Tree, "Dump", output);
    fclose(output);

    treeDestructor(Tree);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
