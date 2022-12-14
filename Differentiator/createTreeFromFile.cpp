#include <ctype.h>
#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// static void checkWrongMinuses (node *Node)
// {
//     if (Node->type == OPERATION && Node->data.operation == SUB &&               // Current node is SUB
//         Node->right != NULL &&                                                  // Right node exists
//         Node->right->type == OPERATION && Node->right->data.operation == SUB && // Right node is SUB
//         Node->right->left != NULL &&                                            // Left node of right node exists.
//         Node->right->left->type == VALUE)                                       // Left node of right node is value.
//     {
//         Node->data = {.operation = ADD};
//         Node->right->left->data = {.value = -1 * Node->right->left->data.value};
//     }

//     if (Node->left != NULL)
//         checkWrongMinuses(Node->left);

//     if (Node->right != NULL)
//         checkWrongMinuses(Node->right);

//     return;
// }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getExpressionPlus (node **Node, size_t *size, char **string);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Grammar scheme:
// Grammar  ::= Express+ '\0'
// Express+ ::= Express- {'+' Express-   }*
// Express- ::= Term     {'-' Term       }*
// Term     ::= Power    {['*' '\'] Power}*
// Power    ::= Binary   {'^' Unary      }*
// Unary    ::= 's''i''n' Parent | ... | Parent
// Parent   ::= '(' Express+ ')' | ['A' - 'z'] | Number
// Number   ::= ['+' '-']? ['0' - '9']+ {'.'['0' - '9']+}?

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const char *simpleCommandLineParser (const int argc, const char *argv[])
{
    CHECKERROR(argv != NULL &&
               "Argv pointer can't be NULL.",
               NULL);

    CHECKERROR(argc != 1 &&
               "You haven't entered file name."
               "Example: ./main function.based", 
               NULL);

    CHECKWARNING(argc == 2 &&
                 "You have entered more than two arguments."
                 "Only second argument is taken as the filename.");

    return argv[1];
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void skipSpaces (char **string)
{
    while (isspace(**string))
        (*string)++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static bool isCurrentSymbol (char **string, char symbol)
{
    if (**string == symbol)
    {
        (*string)++;

        return true;
    }

    return false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getNumber (double *number, char **string)
{
    int length = 0;
    CHECKERROR(sscanf(*string, " %lg%n", number, &length) == 1 &&
               "Get number has errors.", 
               ERROR);

    (*string) += length;
    
    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getParentheses (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    if (isCurrentSymbol(string, '('))
    {
        CHECKERROR(getExpressionPlus(Node, size, string) == NOTERROR &&
                   "Get expression plus has errors.",
                   ERROR);

        CHECKERROR(isCurrentSymbol(string, ')') &&
                   "Get parentheses has errors.", 
                   ERROR);

        return NOTERROR;
    }

    else if (isalpha(**string))
    {
        *Node = nodeConstructor(VARIABLE, {.variable = **string});

        CHECKERROR(*Node != NULL &&
                   "Can't create node for variable.",
                   ALLOCATIONERROR);

        (*size)++;
        (*string)++;

        return NOTERROR;
    }

    else
    {
        double number = 0;

        CHECKERROR(getNumber(&number, string) == NOTERROR &&
                   "Get number has errors.", 
                   ERROR);

        *Node = nodeConstructor(VALUE, {.value = number});

        CHECKERROR(*Node != NULL &&
                   "Can't create node for value.",
                   ALLOCATIONERROR);

        (*size)++;
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getUnary (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    #define DEFINE_OPERATION(Name, String, ...)                                        \
        if (strncmp(*string, String, strlen(String)) == 0)                             \
        {                                                                              \
            *string += strlen(String);                                                 \
            *Node = nodeConstructor(OPERATION, {.operation = Name});                   \
            (*size)++;                                                                 \
            if (strncmp(String, "log", 3) == 0)                                        \
            {                                                                          \
                (*Node)->left  = NULL;                                                 \
                (*Node)->right = NULL;                                                 \
                CHECKERROR(getUnary(&(*Node)->left, size, string) == NOTERROR, ERROR); \
                return getUnary(&(*Node)->right, size, string);                        \
            }                                                                          \
            else                                                                       \
                return getUnary(&(*Node)->left, size, string);                         \
        } else

    #include "operations.h"

    // We need only last else
    {
        return getParentheses(Node, size, string);
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getPower (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    node *leftNode = NULL;

    CHECKERROR(getUnary(&leftNode, size, string) == NOTERROR &&
               "Get unary has errors.",
               ERROR);

    skipSpaces(string);

    if (isCurrentSymbol(string, '^'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = POW});

        CHECKERROR(*Node != NULL &&
                   "Can't create node for pow.",
                   ALLOCATIONERROR);

        (*Node)->left = leftNode;
        (*size)++;

        CHECKERROR(getPower(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression has errors.",
                   ERROR);
    }

    else
        *Node = leftNode;

    return NOTERROR; 
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getTerm (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    node *leftNode = NULL;

    CHECKERROR(getPower(&leftNode, size, string) == NOTERROR &&
               "Get power has errors.",
               ERROR);

    skipSpaces(string);

    if (isCurrentSymbol(string, '*'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = MUL}); 

        CHECKERROR(*Node != NULL &&
                   "Can't create node for mul.",
                   ALLOCATIONERROR);

        (*Node)->left = leftNode; 
        (*size)++;

        CHECKERROR(getTerm(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression has errors.",
                   ERROR);
    }

    else if (isCurrentSymbol(string, '/'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = DIV});
        (*Node)->left = leftNode;
        (*size)++;

        CHECKERROR(getTerm(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression has errors.",
                   ERROR);
    }

    else
        *Node = leftNode;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getExpressionMinus (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    node *newNode = NULL;

    CHECKERROR(getTerm(&newNode, size, string) == NOTERROR && 
               "Get term has errors.",
               ERROR);

    skipSpaces(string);

    if (isCurrentSymbol(string, '-'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = SUB}); 

        CHECKERROR(*Node != NULL &&
                   "Can't create node for sub.",
                   ALLOCATIONERROR);

        (*Node)->left = newNode;
        (*size)++;

        CHECKERROR(getExpressionMinus(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression minus has errors.",
                   ERROR);
    }

    else
        *Node = newNode;
    
    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getExpressionPlus (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    node *newNode = NULL;

    CHECKERROR(getExpressionMinus(&newNode, size, string) == NOTERROR && 
               "Get expression minus has errors.",
               ERROR);

    skipSpaces(string);

    if (isCurrentSymbol(string, '+'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = ADD}); 

        CHECKERROR(*Node != NULL &&
                   "Can't create node for add.",
                   ALLOCATIONERROR);

        (*Node)->left = newNode;
        (*size)++;

        CHECKERROR(getExpressionPlus(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression plus has errors.",
                   ERROR);
    }

    else
        *Node = newNode;
    
    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getGrammar (node **Node, size_t *size, char **string)
{
    CHECKERROR(getExpressionPlus(Node, size, string) == NOTERROR &&
               "Get expression has errors",
               ERROR);

    CHECKERROR(isCurrentSymbol(string, '\0') &&
               "String isn't pointer to '\0'",
               ERROR);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *parseFile (const char *filename)
{
    CHECKERROR(filename != NULL &&
               "Can't open file with nullpointer to filename.",
               NULL);

    FILE *file = fopen(filename, "r");

    CHECKERROR(file != NULL &&
               "Can't open file.",
               NULL);

    char *string = NULL;
    fscanf(file, "%m[^\n]s", &string);

    CHECKERROR(string != NULL &&
               "Can't allocate memory for string.",
               NULL);

    fclose(file);

    char *pointer = string;

    tree *Tree = treeConstructor;

    CHECKERROR(Tree != NULL &&
               "Can't create tree.",
               NULL);

    ISERROR getGrammarError = getGrammar(&(Tree->root), &(Tree->size), &string);

    free(pointer);
    pointer = NULL;

    if (getGrammarError != NOTERROR)
    {
        PUTERROR("Can't create tree from file.");
        treeDestructor(Tree);
        return NULL;
    }

    // checkWrongMinuses(Tree->root);

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
