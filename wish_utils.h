#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include "parallel_commands.h"

typedef struct wish_state {
    char* path;
} wish_state;

extern wish_state* shell_state;

void init_wish_state();
void add_shell_path(char* path);
void destroy_wish_state();

void prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input);
void cd_(command_array* cmd);
void path_(command_array* cmd);

void batch_mode();
void interactive_mode();