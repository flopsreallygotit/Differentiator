#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    const char *filename = simpleCommandLineParser(argc, argv);

    CHECKERROR(filename != NULL &&
               "Error in filename parsing.", 
               -1);

    FILE *output = fopen("Differentiator.htm", "w+");

    tree *Tree = parseFile(filename);
    CHECKERROR(Tree != NULL &&
               "Error in file parsing.", 
               -1);

    treeDump(Tree, "Dump of readden tree.", output);

    // tree *Diff = differentiateTree(Tree);
    // CHECKERROR(Diff != NULL &&
    //            "Error in tree differentiation.", 
    //            -1);

    treeDestructor(Tree);

    // treeDump(Diff, "Dump of differentiated tree.", output);

    // treeDestructor(Diff);

    fclose(output);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
