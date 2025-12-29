#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>

typedef enum { Lexer, Parser, Semantic, Codegen, Generic } Category;

typedef enum { INFO = 1, WARNING, ERROR, CRITICAL } Severity;

typedef struct {
    const char* message;
    size_t line;
    size_t collumn;
    size_t length;
    Severity severity;
    Category category;
} Error;

typedef struct {
    Error* errors;  // Heap allocated array of Error's
    size_t size;
    size_t capacity;
} ErrorList;

// Allocates, Creates, Initializes, and returns the initial error list pointer
ErrorList* create_errors();

/* Construct and push an error to the provided error list */
ErrorList* push_error(ErrorList* error_list, const char* message, Severity severity, size_t line,
                      size_t column, Category category);

void free_errors(ErrorList* error_list);

#endif /* ERROR_H */
