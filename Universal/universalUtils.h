#ifndef UNIVERSAL_UTILS_H
#define UNIVERSAL_UTILS_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <stdio.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Colors for scanf and printf.

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Text features for html.

#define HTMLBOLDRESET "</b>"
#define HTMLBOLD      "<b>"

#define HTMLRESET     "</p>"
#define HTMLGREEN     "<p style=\"color:green;\">"
#define HTMLBLUE      "<p style=\"color:blue;\">"
#define HTMLRED       "<p style=\"color:red;\">"

#define VERDANA       "<p style=\"font-family:verdana; font-size:200%%;\">"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum ISERROR
{
    /// @brief Shows that function fulfilled without errors.
    NOTERROR         = 0,

    /// @brief Pointer is NULL.
    NULLPOINTER      = 1,

    /// @brief Tree is wrong.
    WRONGTREE        = 2,

    /// @brief Tree size is wrong or capacity is bigger than maxSize.
    WRONGSIZE        = 3,

    /// @brief General error.
    ERROR            = 4,

    /// @brief Error in memory allocation.
    ALLOCATIONERROR  = 5,

    /// @brief File content is wrong.
    FILECONTENTERROR = 6,

    /// @brief Value of variable is NAN, inf or overflow max limit of it type.
    ISNOTFINITE      = 7,

    /// @brief Wrong sum. 
    WRONGSUM         = 8,

    /// @brief Pop from empty stack.
    POPOUTEMPTY      = 9,

    /// @brief Left canary died.
    LEFTCANARY       = 10,

    /// @brief Right canary died.
    RIGHTCANARY      = 11,

    /// @brief Data left canary.
    DATALEFTCANARY   = 12,

    /// @brief Data right canary died.
    DATARIGHTCANARY  = 13,

    /// @brief Stack is wrong.
    WRONGSTACK       = 14,

    /// @brief Something is empty.
    EMPTY            = 15,
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Puts warning in stderr.
#define PUTWARNING(warning)                                                                \
    fprintf(stderr, BOLD YELLOW "Warning in file %s:%d line in function: %s! %s;\n" RESET, \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, warning)                              \

/// @brief Puts warning if condition is false.
/// @param condition Condition that you need to check.
#define CHECKWARNING(condition)     \
    do                              \
    {                               \
        if (!(condition))           \
            PUTWARNING(#condition); \
    }                               \
    while (0)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Puts error in stderr.
#define PUTERROR(error)                                                               \
    fprintf(stderr, BOLD RED "Error in file %s:%d line in function: %s! %s;\n" RESET, \
            __FILE__, __LINE__, __PRETTY_FUNCTION__, error)                           \

/// @brief Returns returnValue if condition is wrong.
/// @param condition Condition that you need to check.
/// @param value Value that function returns.
#define CHECKERROR(condition, value) \
    do                               \
    {                                \
        if (!(condition))            \
        {                            \
            PUTERROR(#condition);    \
            return (value);          \
        }                            \
    }                                \
    while (0)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Check that function returned NOTERROR (Zero / false) value and returns -1 if it false.
#define DOTHIS(value)                        \
    do                                       \
    {                                        \
        CHECKERROR((value) == NOTERROR, -1); \
    }                                        \
    while (0)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Reallocates memory for pointer and nulling it.
/// @param ptr Pointer.
/// @param newSize New pointer memory size.
void *recalloc (void *ptr, size_t newSize);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 

/// @brief Calls system with formatted text.
/// @param format Format of arguments (printf).
int systemf (const char *format, ...);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @return sign(firstNumber - secondNumber)
int differenceSign (double firstNumber, double secondNumber);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
