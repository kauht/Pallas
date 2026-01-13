#include "diagnostic.h"

#include <stdlib.h>
#include <string.h>

ErrorList* create_errors() {
    ErrorList* error_list = (ErrorList*)malloc(sizeof(ErrorList));
    if (!error_list) {
        return NULL;
    }

    error_list->size = 0;
    error_list->capacity = 0;
    error_list->errors = NULL;
    return error_list;
}

// TODO: Make realloc growth more efficient, when size >= capacity, realloc
// capacity 2x.
ErrorList* push_error(ErrorList* error_list, const char* message, Severity severity, size_t line,
                      size_t column, Category category) {
    if (!message || !error_list) {
        return NULL;
    }

    size_t msg_len = strlen(message);
    char* msg_cpy = (char*)malloc(msg_len + 1);
    if (!msg_cpy) {
        return NULL;
    }
    memcpy(msg_cpy, message, msg_len + 1);
    Error error = {msg_cpy, line, column, msg_len, severity, category};

    Error* temp = (Error*)realloc(error_list->errors, sizeof(Error) * (error_list->size + 1));
    if (!temp) {
        free(msg_cpy);
        return NULL;
    }
    temp[error_list->size] = error;
    error_list->errors = temp;
    error_list->size++;

    return error_list;
}

void free_errors(ErrorList* error_list) {
    if (!error_list)
        return;
    for (size_t i = 0; i < error_list->size; i++) {
        free((void*)error_list->errors[i].message);
    }
    free(error_list->errors);
    free(error_list);
}
