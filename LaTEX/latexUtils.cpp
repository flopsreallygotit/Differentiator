#include <stdlib.h>
#include <stdio.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <sys/stat.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "universalUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR createBuffer (FILE *file, char **buffer)
{
    size_t size = 0;

    struct stat fileStatBuffer;
    fstat(fileno(file), &fileStatBuffer);
    size = fileStatBuffer.st_size;

    *buffer = (char *) calloc (size + 1, sizeof(char));

    CHECKERROR(*buffer != NULL &&
               "Can't allocate memory for buffer.",
               ALLOCATIONERROR);

    fread(*buffer, sizeof(char), size, file);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static size_t getLineCount (const char *buffer)
{
    size_t lineCount = 0;

    for (size_t buffer_idx = 0; buffer[buffer_idx] != '\0'; buffer_idx++)
        if (buffer[buffer_idx] == '\n')
            lineCount++;

    return lineCount;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR parseFile (const char *filename, size_t *lineCount)
{
    FILE *file = fopen(filename, "r");

    CHECKERROR(file != NULL &&
               "Can't open file.",
               ALLOCATIONERROR);

    char *buffer = NULL;
    CHECKERROR(createBuffer(file, &buffer) == NOTERROR && 
               "Can't create buffer.", 
               ERROR);

    char *phrases = 
    phrases[0] = buffer;


    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
