#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const char *simpleCommandLineParser (const int argc, const char *argv[])
{
    CHECKERROR(argv != NULL &&
               "Argv pointer can't be NULL.",
               NULL);

    CHECKERROR(argc != 1 &&
               "You haven't entered file name."
               "Example: ./main base.txt", 
               NULL);

    CHECKWARNING(argc == 2 &&
                 "You have entered more than two arguments."
                 "Only second argument is taken as the filename.");

    return argv[1];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static node* nodeReadFromFile (FILE *file)
{   
    char firstSymbol = 0;
    fscanf(file, " %c", &firstSymbol);
    
    if ((char) firstSymbol == '(')
    {
        node *currentNode = scanData(file);

        if (currentNode != NULL)
        {
            char lastSymbol = 0;
            fscanf(file, " %c", &lastSymbol);

            // If it is leaf.
            if (lastSymbol == ')')
                return currentNode;
            
            ungetc(lastSymbol, file);

            // Pushing to left  subtree.
            pushLeafToNode(currentNode, nodeReadFromFile(currentNode, file));

            // Pushing to right subtree.
            pushLeafToNode(currentNode, nodeReadFromFile(currentNode, file));

            char sequenceEndSymbol = 0;
            fscanf(file, " %c", &sequenceEndSymbol);

            if (fgetc(file) != ')')
            {
                PUTERROR("Incorrect bracket sequence.");

                return NULL;
            }
        }
    }
  
    return NULL;
}

// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *parseFile (const char *filename)
{
    CHECKERROR(filename != NULL &&
               "Can't open file with nullpointer to filename.",
               NULL);

    FILE *file = fopen(filename, "r");

    CHECKERROR(file != NULL &&
               "Can't open file.",
               NULL);

    tree *Tree = treeConstructor;

    CHECKERROR(Tree != NULL &&
               "Can't create tree.",
               NULL);

    insertRootAsNode(Tree, nodeReadFromFile(file));

    fclose(file);

    return Tree;
}

// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
