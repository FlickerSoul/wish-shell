#include <stdlib.h>
#include <string.h>


typedef struct command_array {
    unsigned int length;
    unsigned int current;
    char** commands;
} command_array;

command_array* new_command_arr(int size, int current);
command_array* quick_new_command_arr();
void free_command_arr(command_array* command_arr);
command_array* resize_command_arr(command_array* old_arr);
void push_command(command_array** command_arr_ptr, char* command);