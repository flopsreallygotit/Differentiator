#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

node *nodeConstructor (TYPE type, data_t data)
{                                          
    node *Node = (node *) calloc (1, sizeof(node));

    CHECKERROR(Node != NULL && 
               "Can't allocate memory for node.", 
               NULL); 

    Node->data  = data;
    Node->type  = type;

    Node->left  = NULL;                                               
    Node->right = NULL;

    return Node;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

tree *treeConstructorFunction (const char *filename, const int line)
{
    tree *Tree = (tree *) calloc (1, sizeof(tree));

    CHECKERROR(Tree != NULL && 
               "Can't allocate memory for tree.",
               NULL);

    Tree->size = 0;
    Tree->root = nodeConstructor(UNKNOWN, {0});

    CHECKERROR(Tree->root != NULL, NULL);

    #ifdef BIRTHINFO

    Tree->birthFile = filename;
    Tree->birthLine = line;

    #endif

    return Tree;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursiveTreeDestroyer (node *Node)
{
    if (Node == NULL)
        return;

    if (Node->left  != NULL)
        recursiveTreeDestroyer(Node->left);
    
    if (Node->right != NULL)
        recursiveTreeDestroyer(Node->right);

    Node->left   = NULL;
    Node->right  = NULL;

    free(Node);
    Node = NULL;

    return;
}

void treeDestructor (tree *Tree)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to destroy nullpointer.", 
               (void) NULL);

    Tree->size = 0;

    recursiveTreeDestroyer(Tree->root);

    free(Tree);
    Tree = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR insertRoot (tree *Tree, TYPE type, data_t data)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    free(Tree->root);
    Tree->root = nodeConstructor(type, data);

    Tree->size++;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR insertRootAsNode (tree *Tree, node *Root)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    free(Tree->root);
    Tree->root = Root;

    Tree->size++;

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR insertLeafToNode (tree *Tree, node *Node, node *Leaf)
{
    CHECKERROR(Tree != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);

    CHECKERROR(Node != NULL && 
               "You are trying to push in nullpointer.", 
               NULLPOINTER);
    
    // Leaf can be NULL.

    Tree->size++;

    if (Node->left == NULL)
        Node->left  = Leaf;

    else if (Node->right == NULL)
        Node->right = Leaf;
    
    else
        CHECKERROR(NULL &&
                   "Both nodes aren't NULL. Can't push.",
                   ERROR);

    return NOTERROR;    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void elementOutput (const TYPE type, const data_t element, 
                           bool putQuotes, FILE *file)
{
    if (putQuotes)
        putc('"', file);

    switch(type)
    {
        case VALUE:
            fprintf(file, "%lg", element.value);
            break;

        case VARIABLE:
            fprintf(file, "%c", element.variable);
            break;

        case OPERATION:
            fprintf(file, "%c",  element.operation);
            break;

        default:
            fprintf(file, "Err");
            break;
    }

    if (putQuotes)
        putc('"', file);
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void recursiveInorderPrintTree (const node *Node, FILE *file)
{
    putc('(', file);

    if (Node->left  != NULL)
        recursiveInorderPrintTree(Node->left,  file);

    elementOutput(Node->type, Node->data, false, file);

    if (Node->right != NULL)
        recursiveInorderPrintTree(Node->right, file);

    putc(')', file);

    return;
}

ISERROR inorderPrintTree (tree *Tree, FILE *file)
{
    CHECKERROR(Tree != NULL &&
               "Can't print nullpointer.",
               NULLPOINTER);

    recursiveInorderPrintTree(Tree->root, file);

    putc('\n', file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define PRINTNODE(Node, file)                                      \
    do                                                             \
    {                                                              \
        fprintf(file, "    Node   pointer: " "%p\n", Node);        \
        fprintf(file, "    Left   pointer: " "%p\n", Node->left);  \
        fprintf(file, "    Right  pointer: " "%p\n", Node->right); \
        fprintf(file, "    Element: ");                            \
        elementOutput(Node->type, Node->data, true, file);         \
        fprintf(file, "\n\n");                                     \
    }                                                              \
    while (0)

static void recursivePrintTree (const node *Node, FILE *file)
{
    PRINTNODE(Node, file);

    if (Node->left  != NULL)
        recursivePrintTree(Node->left,  file);

    if (Node->right != NULL)
        recursivePrintTree(Node->right, file);

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR simpleTreeDumpFunction (const tree *Tree, const char *treename, const char *filename, 
                                       const int   line, const char *function, FILE *file)
{
    CHECKERROR(Tree     != NULL && 
               "Can't dump nullpointer.", 
               NULLPOINTER);

    CHECKERROR(treename != NULL &&
               "Can't dump with nullpointer to treename.",
               NULLPOINTER);

    CHECKERROR(filename != NULL &&
               "Can't dump with nullpointer to filename.",
               NULLPOINTER);

    CHECKERROR(function != NULL &&
               "Can't dump with nullpointer to function.", 
               NULLPOINTER);

    CHECKERROR(file     != NULL &&
               "Can't dump with nullpointer to file.", 
               NULLPOINTER);

    fprintf(file, HTMLBLUE "\nTree dump at %s:%d in %s\n" HTMLRESET 
            HTMLBOLD "Tree %s[%p] " HTMLBOLDRESET, 
            filename, line, function, treename, Tree);

    fprintf(file, "{\n");

    putc('\n', file);

    if (Tree->root)
        recursivePrintTree(Tree->root, file);

    else
        fprintf(file, "    Tree hasn't root.\n");

    fprintf(file, "}\n\n");

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Only address of current node or it's parent allow us to distinguish them.
// So I use address for this.

#define DEFINENODE(NodeParent, Node, tmp)                                              \
    do                                                                                 \
    {                                                                                  \
        if (Node != NULL)                                                              \
        {                                                                              \
            fprintf(tmp, "    node%p [shape=box3d, style=\"filled\" fillcolor=\"%s\"," \
                    "fontname=\"Arial\", label = ",                                    \
                    Node, globalFillColor);                                            \
            elementOutput(Node->type, Node->data, true, tmp);                          \
            fprintf(tmp, "]\n");                                                       \
            fprintf(tmp, "    node%p -> node%p\n", NodeParent, Node);                  \
        }                                                                              \
    }                                                                                  \
    while (0)

static void recursiveFileFiller (const node *Node, FILE *tmp)
{
    DEFINENODE(Node, Node->left,  tmp);
    DEFINENODE(Node, Node->right, tmp);

    if (Node->left  != NULL)
        recursiveFileFiller(Node->left,  tmp);

    if (Node->right != NULL)
        recursiveFileFiller(Node->right, tmp);
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void fillTemporaryFile (const tree *Tree, FILE *tmp)
{

    fprintf(tmp, 
            "digraph BST\n{\n"
            "    node%p [shape=box3d, style=\"filled\" fillcolor=\"%s\","
            "fontname=\"Arial\", label = ",
            Tree->root, globalFillColor);
    
    elementOutput(Tree->root->type, Tree->root->data, true, tmp);
    fprintf(tmp, "]\n");
    
    recursiveFileFiller(Tree->root, tmp);

    putc('}', tmp);  
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Html output need sources of images.
// => We need to save all dump images.
// => We need a number of current image:

static int dumpNumber = 0;

static void makeSourcesImages(void)
{
    systemf("dot ./TreeDumpSources/tmp.txt -T png -o ./TreeDumpSources/graph%d.png", dumpNumber);

    return;
}

ISERROR treeDumpFunction (const tree *Tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file)
{
    CHECKERROR(Tree     != NULL && 
               "Can't dump nullpointer.", 
               NULLPOINTER);

    CHECKERROR(message  != NULL && 
               "Can't dump with nullpointer to message.", 
               NULLPOINTER);

    CHECKERROR(treename != NULL &&
               "Can't dump with nullpointer to treename.",
               NULLPOINTER);

    CHECKERROR(filename != NULL &&
               "Can't dump with nullpointer to filename.",
               NULLPOINTER);

    CHECKERROR(function != NULL &&
               "Can't dump with nullpointer to function.", 
               NULLPOINTER);

    CHECKERROR(file     != NULL &&
               "Can't dump with nullpointer to file.", 
               NULLPOINTER);

    fprintf(file, 
            "\n<hr>\n<pre>\n"
            "<h2>%s</h2>\n"
            VERDANA, 
            message);

    ISERROR treeError = simpleTreeDumpFunction(Tree, treename, filename,
                                               line, function, file);
    fprintf(file, "</p>\n");

    systemf("mkdir -p ./TreeDumpSources/");

    FILE *tmp = fopen("./TreeDumpSources/tmp.txt", "w");

    CHECKERROR(tmp != NULL &&
               "Can't open temporary file for graph dump.",
               ALLOCATIONERROR);

    fillTemporaryFile(Tree, tmp);
    fclose(tmp);

    makeSourcesImages();

    fprintf(file, "<img src = ./TreeDumpSources/graph%d.png>\n", dumpNumber);
    dumpNumber++;

    systemf("rm -rf ./TreeDumpSources/tmp.txt");

    return treeError;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
