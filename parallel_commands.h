#include <stdbool.h>
#include "commands.h"

const char* operators[] = {">", "&"};
unsigned short operator_num = 2;
bool is_operator(char* part);

typedef struct parallel_commands {
    unsigned int length;
    unsigned int current;
    command_array** command_arrays;
} parallel_commands;

parallel_commands* new_parallel_commands(int size, int current);
parallel_commands* quick_new_parallel_commands();
void free_parallel_commands(parallel_commands* pc);
void free_parallel_commands_and_all(parallel_commands** pc_ptr);
void resize_parallel_commands(parallel_commands** old_cmds_ptr);
void push_command_array(parallel_commands** pc_ptr, command_array* new_command_arr);
void parse_command(parallel_commands** pc_ptr, char* new_line);
