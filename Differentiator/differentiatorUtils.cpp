#include <ctype.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

#define PROCESS_OPERATION_CASE(symbol, operation_data) \
    case symbol:                                       \
        Node->type = OPERATION;                        \
        Node->data = {.operation = operation_data};    \
        break

static ISERROR fillNode (node *Node, FILE *file)
{
    char symbol = 0;

    fscanf(file, " %c", &symbol);

    switch (symbol)
    {
        PROCESS_OPERATION_CASE('+', ADD);
        PROCESS_OPERATION_CASE('-', SUB);
        PROCESS_OPERATION_CASE('*', MUL);
        PROCESS_OPERATION_CASE('/', DIV);
        PROCESS_OPERATION_CASE('^', POW);
        
        default:
            break;
    }

    if (Node->type == UNKNOWN && isalpha(symbol))
    {
        Node->type = VARIABLE;
        Node->data = {.variable = symbol};
    }

    else if (Node->type == UNKNOWN)
    {
        ungetc(symbol, file);

        double number = 0;
        int count = fscanf(file, " %lg", &number);

        if (count != 0)
        {
            Node->type = VALUE;
            Node->data = {.value = number};
        }
    }

    if (Node->type == UNKNOWN)
        return ERROR;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// static node *nodeReadFromFile (node *Node, FILE *file)
// {   
//     char firstSymbol = 0;
//     fscanf(file, " %c", &firstSymbol);
    
//     if (firstSymbol == '(')
//     {
//         node *Node = nodeConstructor(UNKNOWN, {0});

//         nodeReadFromFile(Node->left,  file);

//         fillNode(Node, file);

//         printf("TYPE %d", Node->type);

//         char lastSymbol = 0;
//         scanf(" %c", &lastSymbol);

//         ungetc(lastSymbol, file);

//         if (lastSymbol == ')')
//             return Node;

//         nodeReadFromFile(Node->right, file);
//     }

//     if (firstSymbol != ')')
//     {
//         fillNode(Node, file);

//         return Node;
//     }
  
//     return NULL;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// tree *parseFile (const char *filename)
// {
//     CHECKERROR(filename != NULL &&
//                "Can't open file with nullpointer to filename.",
//                NULL);

//     FILE *file = fopen(filename, "r");

//     CHECKERROR(file != NULL &&
//                "Can't open file.",
//                NULL);

//     tree *Tree = treeConstructor;

//     CHECKERROR(Tree != NULL &&
//                "Can't create tree.",
//                NULL);

//     fclose(file);

//     return Tree;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
