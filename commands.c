#include "commands.h"

command_array* new_command_arr(int size, int current) {
    command_array* command_arr = malloc(sizeof(command_array));
    if (command_arr == NULL) {
        return NULL;
    }
    command_arr->length = size;
    command_arr -> current = current;
    command_arr->commands = malloc(command_arr->length * (sizeof(char*)));
    return command_arr;
}

command_array* quick_new_command_arr() {
    return new_command_arr(10, 0);
}

void free_command_arr(command_array* command_arr) {
    free(command_arr->commands);
    free(command_arr);
}

void free_all_commands_and_arr(command_array** command_arr_ptr) {
    if (command_arr_ptr == NULL || *command_arr_ptr == NULL) {
        return;
    }

    command_array* command_arr = *command_arr_ptr;

    for (int i = 0; i < command_arr->current; i++) {
        free(command_arr->commands[i]);
    }
    free_command_arr(command_arr);
    *command_arr_ptr = NULL;
}

void resize_command_arr(command_array** old_arr_ptr) {
    if (old_arr_ptr == NULL || *old_arr_ptr == NULL) {
        return;
    }

    command_array* old_arr = *old_arr_ptr;

    command_array* new_arr = new_command_arr(old_arr->length * 2, old_arr->current);
    if (new_arr == NULL) {
        return;
    }

    for (int i = 0; i < old_arr->current; i++) {
        new_arr->commands[i] = old_arr->commands[i];
    }
    free_command_arr(old_arr);
    *old_arr_ptr = new_arr;
}

void push_command(command_array** command_arr_ptr, char* command) {
    if (command_arr_ptr == NULL || *command_arr_ptr == NULL) {
        return;
    }
    command_array* command_arr = *command_arr_ptr;
    command_arr->commands[command_arr->current++] = command;
    if (command_arr->current == command_arr->length) {
        resize_command_arr(command_arr_ptr);
    }
}
