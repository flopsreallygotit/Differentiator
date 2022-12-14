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

    DOTHIS(treeDump(Tree, "Dump of readden tree.", output));
    DOTHIS(simplifyTree(Tree));
    DOTHIS(treeDump(Tree, "Dump of tree after simplifier.", output));

    latex *Latex = latexConstructor("LaTEX/phrases.txt", "Differentiator.tex");
    CHECKERROR(Latex != NULL &&
               "Can't create latex struct.",
               -1);

    tree *Diff = differentiateTree(Tree);
    CHECKERROR(Diff != NULL &&
               "Error in tree differentiation.", 
               -1);

    DOTHIS(treeDestructor(Tree));

    DOTHIS(treeDump(Diff, "Dump of differentiated tree.", output));
    DOTHIS(simplifyTree(Diff));
    DOTHIS(treeDump(Diff, "Dump of differentiated tree after simplifier.", output)); 

    fclose(output);

    latexDestructor(Latex);

    DOTHIS(treeDestructor(Diff));

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
