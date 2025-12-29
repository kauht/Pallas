#include "errors.h"
#include <stdio.h>

int main() {
    ErrorList* error_list = create_errors();
    push_error(error_list, "Yo what's good", INFO, 1, 1, Generic);

    printf("%s", error_list->errors[0].message);

    free_errors(error_list);
}
