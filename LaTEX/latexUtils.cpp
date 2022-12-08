#include <stdlib.h>
#include <sys/stat.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "latexUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void getSymbolsCount (latex *Latex)
{
    struct stat fileStatBuffer;
    fstat(fileno(Latex->input), &fileStatBuffer);
    Latex->symbolsCount = fileStatBuffer.st_size;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void getLineCount (latex *Latex)
{
    for (size_t buffer_idx = 0; Latex->buffer[buffer_idx] != '\0'; buffer_idx++)
        if (Latex->buffer[buffer_idx] == '\n')
            Latex->lineCount++;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR createBuffer (latex *Latex)
{
    getSymbolsCount(Latex);

    Latex->buffer = (char *) calloc (Latex->symbolsCount + 1, sizeof(char));

    CHECKERROR(Latex->buffer != NULL &&
               "Can't allocate memory for buffer.",
               ALLOCATIONERROR);

    fread(Latex->buffer, sizeof(char), Latex->symbolsCount, Latex->input);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static ISERROR makePhrases(latex *Latex)
{
    getLineCount(Latex);

    Latex->phrases = (char **) calloc (Latex->symbolsCount, sizeof(char));

    CHECKERROR(Latex->phrases != NULL &&
               "Can't allocate memory for phrases array.",
               ALLOCATIONERROR);

    size_t phrase_idx = 0;
    bool isNewString  = true;

    for (size_t buffer_idx = 0; Latex->buffer[buffer_idx] != '\0'; buffer_idx++)
    {
        if (isNewString)
        {
            Latex->phrases[phrase_idx] = Latex->buffer + buffer_idx;
            phrase_idx++;

            isNewString = false;
        }

        if (Latex->buffer[buffer_idx] == '\n')
            isNewString = true;
    }

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

latex *latexConstructor (const char *input, const char *output)
{
    CHECKERROR(input != NULL &&
               "Can't create latex struct with null input.",
               NULL);

    CHECKERROR(input != NULL &&
               "Can't create latex struct with null output.",
               NULL);

    latex *Latex = (latex *) calloc (1, sizeof(latex));

    CHECKERROR(Latex != NULL &&
               "Can't allocate memory for latex struct.",
               NULL);

    Latex->input  = fopen(input,  "r");
    Latex->output = fopen(output, "w+");

    CHECKERROR(createBuffer(Latex) == NOTERROR &&
               "Can't create buffer for latex struct.", 
               NULL);

    CHECKERROR(makePhrases(Latex) == NOTERROR &&
               "Can't make phrases for latex struct.", 
               NULL);

    return Latex;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void latexDestructor (latex *Latex)
{
    if (Latex == NULL)
        return;

    fclose(Latex->input);
    fclose(Latex->output);

    free(Latex->buffer);
    free(Latex->phrases);

    Latex->buffer  = NULL;
    Latex->phrases = NULL;

    Latex->symbolsCount = 0;
    Latex->lineCount    = 0;

    free(Latex);
    Latex = NULL;

    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ISERROR printRandomPhrase (latex *Latex)
{
    size_t phrase_idx = rand() % Latex->lineCount;

    for (size_t symbol_idx = 0; Latex->phrases[phrase_idx][symbol_idx] != '\n'; symbol_idx++)
        putc(Latex->phrases[phrase_idx][symbol_idx], Latex->output);

    putc('\n', Latex->output);

    return NOTERROR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
