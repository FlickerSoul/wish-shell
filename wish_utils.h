#include <stdio.h>
#include <unistd.h>
#include "parallel_commands.h"

typedef struct wish_state {
    char* path;
} wish_state;

void init_wish_state();
void destroy_wish_state();

void prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input);

void batch_mode();
void interactive_mode();