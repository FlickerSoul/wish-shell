#include "parallel_commands.h"
#include <stdio.h>

const char operators[] = {' ', '>', '&'};
const char* operator_map[] = {" ", ">", "&"};
unsigned short operator_num = 3;

const char* is_operator(char part) {
    for (int i = 0; i < operator_num; i++) {
        if (part == operators[i]) {
            return operator_map[i];
        }
    }
    return NULL;
}

parallel_commands* new_parallel_commands(int size, int current) {
    parallel_commands* pc = malloc(sizeof(parallel_commands));
    if (pc == NULL) {
        return NULL;
    }
    pc->length = size;
    pc->current = current;
    pc->command_arrays = malloc(pc->length * (sizeof(command_array*)));
    return pc;
}

parallel_commands* quick_new_parallel_commands() {
    return new_parallel_commands(2, 0);
}

void free_parallel_commands_and_all(parallel_commands** pc_ptr) {
    if (pc_ptr == NULL || *pc_ptr == NULL) {
        return;
    }

    parallel_commands* pc = *pc_ptr;

    for (int i = 0; i < pc->current; i++) {
        free_all_commands_and_arr(&pc->command_arrays[i]);
    }
    free(pc->command_arrays);
    free(pc);
    *pc_ptr = NULL;
}

void resize_parallel_commands(parallel_commands* pc) {
    if (pc == NULL) {
        return;
    }

    pc->length *= 2;
    
    command_array** new_cmd_arrs = malloc(pc->length * sizeof(command_array*));

    for (int i = 0; i < pc->current; i++) {
        new_cmd_arrs[i] = pc->command_arrays[i];
    }
    free(pc->command_arrays);
    pc->command_arrays = new_cmd_arrs;
}

void push_command_array(parallel_commands** pc_ptr, command_array* new_command_arr) {
    if (pc_ptr == NULL || *pc_ptr == NULL) {
        return;
    }
    parallel_commands* pc = *pc_ptr;
    pc->command_arrays[pc->current++] = new_command_arr;
    if (pc->current == pc->length) {
        resize_parallel_commands(*pc_ptr);
    }
}

command_array* parse_operator(command_array* command_arr, char operator, char** last_part_ptr) {
    if (operator == '&') {
        command_arr = quick_new_command_arr();
    } else if( operator == '>') {
        // if (command_arr->current == 0) {
        //      return NULL;
        // }
        char* sep = strtok_r(*last_part_ptr, " ", last_part_ptr);
        if (sep == NULL) {
            return NULL;
        }
        put_std_out(command_arr, strdup(sep));
    }

    return command_arr;
}

void parse_command(parallel_commands** pc_ptr, char* new_line) {
    // char* last_part = strtok_r(new_line, "", &last_part);
    char* last_part = new_line;
    const char* sep_char = NULL;
    char* sep = NULL;
    command_array* command_arr = quick_new_command_arr();
    command_array* temp_arr = NULL;
    push_command_array(pc_ptr, command_arr);

    int counter = 0;
    while (last_part != NULL && counter < strlen(last_part)) {
        // printf("last part: %s;and len: %lu\n", last_part, strlen(last_part));
        // printf("coutner char: %c\n", last_part[counter]);
        sep_char = is_operator(last_part[counter]);
        // printf("sep_char: %s\n", sep_char);

        if (sep_char != NULL) {
            // if it's an operator 
            // printf("sep_char: %s", sep_char);
            sep = strtok_r(last_part, sep_char, &last_part);
            counter = 0;

            // printf("sep: %s\n", sep);

            if (sep != NULL) {
                // break;
                push_command(&command_arr, strdup(sep));
            }

            temp_arr = parse_operator(command_arr, sep_char[0], &last_part);

            if (temp_arr != command_arr) {
                if (temp_arr == NULL) {
                    printf("operator err\n");
                }
                // printf("new temp arr");
                wrap_up_command(&command_arr);
                command_arr = temp_arr;
                push_command_array(pc_ptr, command_arr);
            }
        } else {
            counter += 1;
        }
        // if it's not pass
    }
    // printf("remaining last part: %s\n", last_part);
    sep = strtok_r(last_part, "", &last_part);
    if (sep != NULL) {
        push_command(&command_arr, strdup(sep));
    }

    wrap_up_command(&command_arr);
    free(new_line);
}