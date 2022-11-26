#include <ctype.h>
#include <string.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "differentiatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getExpression (node **Node, size_t *size, char **string);

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
        ISERROR expressionError = getExpression(Node, size, string);

        CHECKERROR(isCurrentSymbol(string, ')') &&
                   "Get parentheses has errors.", 
                   ERROR);

        return expressionError;
    }

    else if (isalpha(**string))
    {
        *Node = nodeConstructor(VARIABLE, {.variable = **string});
        (*size)++;

        (*string)++;

        return NOTERROR;
    }

    else
    {
        double number = 0;
        ISERROR numberError = getNumber(&number, string);

        *Node = nodeConstructor(VALUE, {.value = number});
        (*size)++;

        return numberError;
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getUnary (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    #define DEFINE_OPERATION(Name, String, ...)                      \
        if (strncmp(*string, String, strlen(String)) == 0)           \
        {                                                            \
            *string += strlen(String);                               \
            *Node = nodeConstructor(OPERATION, {.operation = Name}); \
            (*size)++;                                               \
            return getUnary(&(*Node)->left, size, string);           \
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
        *Node = nodeConstructor(OPERATION, {.operation = MUL}); // TODO copypaste
        (*Node)->left = leftNode; // TODO function ordering
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

static ISERROR getExpression (node **Node, size_t *size, char **string)
{
    skipSpaces(string);

    node *leftNode = NULL;

    CHECKERROR(getTerm(&leftNode, size, string) == NOTERROR &&
               "Get term has errors.",
               ERROR);

    skipSpaces(string);

    if (isCurrentSymbol(string, '+'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = ADD}); // TODO copypaste
        (*Node)->left = leftNode;
        (*size)++;

        CHECKERROR(getExpression(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression has errors.",
                   ERROR);
    }

    else if (isCurrentSymbol(string, '-'))
    {
        *Node = nodeConstructor(OPERATION, {.operation = SUB});
        (*Node)->left = leftNode;
        (*size)++;

        CHECKERROR(getExpression(&(*Node)->right, size, string) == NOTERROR &&
                   "Get expression has errors.",
                   ERROR);
    }

    else
        *Node = leftNode;
    
    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR getGrammar (node **Node, size_t *size, char **string)
{
    CHECKERROR(getExpression(Node, size, string) == NOTERROR &&
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

    getGrammar(&(Tree->root), &(Tree->size), &string);

    free(pointer);
    pointer = NULL;

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
