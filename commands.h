#include <stdlib.h>
#include <string.h>


typedef struct command_array {
    unsigned int length;
    unsigned int current;
    char** commands;
    char* std_out;
} command_array;

command_array* new_command_arr(int size, int current, char* std_out);
command_array* quick_new_command_arr();
void free_command_arr(command_array* command_arr);
void free_all_commands_and_arr(command_array** command_array_ptr);
void resize_command_arr(command_array* old_arr_ptr);
void put_std_out(command_array* command_arr, char* std_out);
void wrap_up_command(command_array** command_arr_ptr);
void push_command(command_array** command_arr_ptr, char* command);
