#include "error.h"

int main() {
    ErrorList* error_list = create_errors();
    push_error(error_list, "Yo what's good", INFO, 1, 1, Generic);
    free_errors(error_list);
}
