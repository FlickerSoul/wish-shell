#include "parallel_commands.h"

const char* operators[] = {">", "&"};
unsigned short operator_num = 2;

bool is_operator(char* part) {
    for (int i = 0; i < operator_num; i++) {
        if (!strcmp(part, operators[i])) {
            return true;
        }
    }
    return false;
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

void free_parallel_commands(parallel_commands* pc) {
    free(pc->command_arrays);
    free(pc);
}

void free_parallel_commands_and_all(parallel_commands** pc_ptr) {
    if (pc_ptr == NULL || *pc_ptr == NULL) {
        return;
    }

    parallel_commands* pc = *pc_ptr;

    for (int i = 0; i < pc->current; i++) {
        free_all_commands_and_arr(&pc->command_arrays[i]);
    }
    free_parallel_commands(pc);
    *pc_ptr = NULL;
}

void resize_parallel_commands(parallel_commands** old_cmds_ptr) {
    if (old_cmds_ptr == NULL || *old_cmds_ptr == NULL) {
        return;
    }

    parallel_commands* old_cmds = *old_cmds_ptr;
    parallel_commands* new_cmds = new_parallel_commands(old_cmds->length * 2, old_cmds->current);

    if (new_cmds == NULL) {
        return;
    }

    for (int i = 0; i < old_cmds->current; i++) {
        new_cmds->command_arrays[i] = old_cmds->command_arrays[i];
    }
    free_parallel_commands(old_cmds);
    *old_cmds_ptr = new_cmds;
}

void push_command_array(parallel_commands** pc_ptr, command_array* new_command_arr) {
    if (pc_ptr == NULL || *pc_ptr == NULL) {
        return;
    }
    parallel_commands* pc = *pc_ptr;
    pc->command_arrays[pc->current++] = new_command_arr;
    if (pc->current == pc->length) {
        resize_parallel_commands(pc_ptr);
    }
}

command_array* parse_operator(command_array* command_arr, char operator, char** last_part_ptr) {
    if (operator == '&') {
        command_arr = quick_new_command_arr();
    } else if( operator == '>') {
        char* sep = strtok_r(*last_part_ptr, " ", last_part_ptr);
        if (sep == NULL) {
            return NULL;
        }
        command_arr->std_out = strdup(sep);
    }

    return command_arr;
}

void parse_command(parallel_commands** pc_ptr, char* new_line) {
    char* last_part = new_line;
    char* sep = NULL;
    command_array* command_arr = quick_new_command_arr();
    command_array* temp_arr = NULL;
    push_command_array(pc_ptr, command_arr);
    while ((sep = strtok_r(last_part, " ", &last_part)) != NULL) {
        // when there is nothing in the the last part
        // sep == NULL; do while makes sure that last 
        // ele in arr in NULL
        if (is_operator(sep)) {
            temp_arr = parse_operator(command_arr, sep[0], &last_part);
            if (temp_arr != command_arr) {
                wrap_up_command(&command_arr);
                command_arr = temp_arr;
                push_command_array(pc_ptr, command_arr);
            }
        } else {
            push_command(&command_arr, strdup(sep));
        }
    }
    wrap_up_command(&command_arr);
    free(new_line);
}