#include "wish_utils.h"
#include "test_utils.h"

void test_prompt_input() {
    char* buffer = NULL;
    size_t size = 0;
    prompt_input(&buffer, &size, stdin);
    assert(compare_string(buffer, "test"));
    free(buffer);

    prompt_input(&buffer, &size, stdin);
    assert(compare_string(buffer, "ls > a"));
    free(buffer);

    prompt_input(&buffer, &size, stdin);
    assert(buffer == NULL);
}

int main() {
    test_prompt_input();
}