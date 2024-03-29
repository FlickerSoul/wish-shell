#include "parallel_commands.h"
#include <stdio.h>

const char operators[] = {' ', '>', '<'};
const char* operator_map[] = {" ", ">", "<"};
unsigned short operator_num = 2;

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
    if ( operator == '>') {

        char* sep = strtok_r(*last_part_ptr, " ", last_part_ptr);

        if (sep == NULL || command_arr->std_out != NULL || (*last_part_ptr != NULL && strcmp(*last_part_ptr, "")) ) {
            return NULL;
        }

        put_std_out(command_arr, strdup(sep));
    } else if (operator == '<') {
        char* sep = strtok_r(*last_part_ptr, " ", last_part_ptr);

        if (sep == NULL) {
            return NULL;
        }

        put_std_in(command_arr, strdup(sep));
    }

    return command_arr;
}

bool parse_command(parallel_commands** pc_ptr, char* new_line) {
    char* line = new_line;
    char* last_part = NULL;
    const char* sep_char = NULL;
    char* sep = NULL;

    command_array* temp_arr = NULL;

    int k = 0;
    while ((last_part = strtok_r(line, "&", &line)) != NULL) {
        command_array* command_arr = quick_new_command_arr();
        push_command_array(pc_ptr, command_arr);
        int counter = 0;
        while (last_part != NULL && counter < strlen(last_part)) {
            sep_char = is_operator(last_part[counter]);

            if (sep_char != NULL) {
                if (counter == 0) {
                    last_part += 1;
                    sep = NULL;
                } else {
                    sep = strtok_r(last_part, sep_char, &last_part);
                }

                counter = 0;

                if (sep != NULL) {
                    push_command(&command_arr, strdup(sep));
                }

                temp_arr = parse_operator(command_arr, sep_char[0], &last_part);

                if (temp_arr == NULL) {
                    return false;

                    wrap_up_command(&command_arr);
                }
            } else {
                counter += 1;
            }
        }

        sep = strtok_r(last_part, "", &last_part);
        if (sep != NULL) {
            push_command(&command_arr, strdup(sep));
        }
        
        wrap_up_command(&command_arr);
    }

    free(new_line);

    return true;
}