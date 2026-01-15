#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    SEVERITY_INFO = 1,
    SEVERITY_WARNING,
    SEVERITY_ERROR,
    SEVERITY_CRITICAL
} Severity;

typedef enum {
    CATEGORY_LEXER = 0,
    CATEGORY_PARSER,
    CATEGORY_SEMANTIC,
    CATEGORY_CODEGEN,
    CATEGORY_GENERIC
} Category;

typedef struct {
    char* filename;
    char* message;
    size_t line;
    size_t column;
    size_t length;
    Severity severity;
    Category category;
} Error;

typedef struct {
    Error* items;
    size_t size;
    size_t capacity;
} ErrorList;

ErrorList* errors_create(void);
void errors_free(ErrorList* list);
void errors_clear(ErrorList* list);

Error* errors_get(ErrorList* list, size_t index);

bool errors_add(ErrorList* list, const char* filename, const char* message, Severity severity,
                size_t line, size_t column, size_t length, Category category);

bool errors_addf(ErrorList* list, const char* filename, Severity severity, size_t line,
                 size_t column, size_t length, Category category, const char* fmt, ...);

char* errors_format(const Error* e);

const char* severity_to_string(Severity s);
const char* category_to_string(Category c);

#endif /* DIAGNOSTIC_H */