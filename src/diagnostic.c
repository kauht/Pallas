#include "diagnostic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ErrorList* errors_create(void) {
    ErrorList* error_list = (ErrorList*)malloc(sizeof(ErrorList));
    if (!error_list) {
        return NULL;
    }

    error_list->size = 0;
    error_list->capacity = 0;
    error_list->items = NULL;
    return error_list;
}

bool errors_add(ErrorList* error_list, const char* filename, const char* message, Severity severity,
                size_t line, size_t column, size_t length, Category category) {
    if (!message || !error_list) {
        return false;
    }

    size_t msg_len = strlen(message);
    char* msg_cpy = (char*)malloc(msg_len + 1);
    if (!msg_cpy) {
        return false;
    }
    memcpy(msg_cpy, message, msg_len + 1);
    
    char* filename_copy = NULL;
    if (filename) {
        size_t fname_len = strlen(filename);
        filename_copy = (char*)malloc(fname_len + 1);
        if (!filename_copy) {
            free(msg_cpy);
            return false;
        }
        memcpy(filename_copy, filename, fname_len + 1);
    }
    
    Error error = {filename_copy, msg_cpy, line, column, length, severity, category};

    Error* temp = (Error*)realloc(error_list->items, sizeof(Error) * (error_list->size + 1));
    if (!temp) {
        free(msg_cpy);
        return false;
    }
    temp[error_list->size] = error;
    error_list->items = temp;
    error_list->size++;

    return true;
}

void errors_free(ErrorList* error_list) {
    if (!error_list)
        return;
    for (size_t i = 0; i < error_list->size; i++) {
        free((void*)error_list->items[i].filename);
        free((void*)error_list->items[i].message);
    }
    free(error_list->items);
    free(error_list);
}

void errors_clear(ErrorList* list) {
    if (!list)
        return;
    for (size_t i = 0; i < list->size; i++) {
        free((void*)list->items[i].filename);
        free((void*)list->items[i].message);
    }
    free(list->items);
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}

Error* errors_get(ErrorList* list, size_t index) {
    if (!list || index >= list->size) {
        return NULL;
    }
    return &list->items[index];
}

bool errors_addf(ErrorList* list, const char* filename, Severity severity, size_t line,
                 size_t column, size_t length, Category category, const char* fmt, ...) {
    if (!list || !fmt) {
        return false;
    }
    
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    return errors_add(list, filename, buffer, severity, line, column, length, category);
}

char* errors_format(const Error* e) {
    if (!e) {
        return NULL;
    }
    
    char buffer[2048];
    int len = 0;
    
    if (e->filename) {
        len = snprintf(buffer, sizeof(buffer), "%s:%zu:%zu: %s: %s",
                      e->filename, e->line, e->column,
                      severity_to_string(e->severity), e->message);
    } else {
        len = snprintf(buffer, sizeof(buffer), "%zu:%zu: %s: %s",
                      e->line, e->column,
                      severity_to_string(e->severity), e->message);
    }
    
    if (len < 0) {
        return NULL;
    }
    
    char* result = (char*)malloc(len + 1);
    if (!result) {
        return NULL;
    }
    memcpy(result, buffer, len + 1);
    return result;
}

const char* severity_to_string(Severity s) {
    switch (s) {
        case SEVERITY_INFO: return "info";
        case SEVERITY_WARNING: return "warning";
        case SEVERITY_ERROR: return "error";
        case SEVERITY_CRITICAL: return "critical";
        default: return "unknown";
    }
}

const char* category_to_string(Category c) {
    switch (c) {
        case CATEGORY_LEXER: return "lexer";
        case CATEGORY_PARSER: return "parser";
        case CATEGORY_SEMANTIC: return "semantic";
        case CATEGORY_CODEGEN: return "codegen";
        case CATEGORY_GENERIC: return "generic";
        default: return "unknown";
    }
}
