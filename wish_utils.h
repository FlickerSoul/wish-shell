#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include "parallel_commands.h"

typedef struct wish_state {
    char** path;
    unsigned int current;
    unsigned int size;
} wish_state;

extern wish_state* shell_state;

void init_wish_state();
void expand_path();
void replace_path();
void add_shell_path(char* path);
void destroy_wish_state();

void prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input);
bool find_cmd(command_array* cmd);
pid_t exec_command(command_array* cmd);
void cd_(command_array* cmd);
void path_(command_array* cmd);

void batch_mode(int, char**);
void interactive_mode();