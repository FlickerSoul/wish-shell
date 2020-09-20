#include "parallel_commands.h"
#include "test_utils.h"

command_array* parse_simple_command_helper(char* new_line) {
    command_array* command_arr = quick_new_command_arr();
    char* last_part = new_line;
    char* sep = NULL;
    while ((sep = strtok_r(last_part, " ", &last_part)) != NULL) {
        // when there is nothing in the the last part
        // sep == NULL; do while makes sure that last 
        // ele in arr in NULL
        push_command(&command_arr, strdup(sep));
    }
    wrap_up_command(&command_arr);
    free(new_line);
    return command_arr;
}

void test_parse_command_helper() {
    char* new_line = strdup("a b c");
    command_array* cmd = parse_simple_command_helper(new_line);
    assert(compare_string(cmd->commands[0], "a"));
    assert(compare_string(cmd->commands[1], "b"));
    assert(compare_string(cmd->commands[2], "c"));
    free_all_commands_and_arr(&cmd);
}

void test_parse_operator() {
    char* new_line = strdup("a b c");
    command_array* cmd = parse_simple_command_helper(new_line);

    char* last_part = "test";
    command_array* new_cmd = parse_operator(cmd, '&', &last_part);
    assert(new_cmd != cmd);
    free_all_commands_and_arr(&new_cmd);

    new_cmd = parse_operator(cmd, '>', &last_part);
    assert(new_cmd == cmd);
    assert(compare_string("test", cmd->std_out));

    free_all_commands_and_arr(&cmd);
}

void test_parallel_commands() {
    parallel_commands* pc = quick_new_parallel_commands();
    free_parallel_commands_and_all(&pc);

    pc = new_parallel_commands(1, 0);
    assert(pc->current == 0);
    assert(pc->length == 1);

    parse_command(&pc, strdup("a b c"));
    assert(pc->current == 1);
    assert(pc->length == 2);

    command_array* cmd = pc->command_arrays[pc->current-1];
    assert(compare_string(cmd->commands[0], "a"));
    assert(compare_string(cmd->commands[1], "b"));
    assert(compare_string(cmd->commands[2], "c"));
    assert(cmd->commands[3] == NULL);
    assert(cmd->current == 4);

    parse_command(&pc, strdup("   d        e f     "));
    assert(pc->current == 2);
    assert(pc->length == 4);

    cmd = pc->command_arrays[pc->current-1];
    assert(compare_string(cmd->commands[0], "d"));
    assert(compare_string(cmd->commands[1], "e"));
    assert(compare_string(cmd->commands[2], "f"));
    assert(cmd->commands[3] == NULL);
    assert(cmd->current == 4);

    parse_command(&pc, strdup("g>h i"));
    assert(pc->current == 3);
    assert(pc->length == 4);

    cmd = pc->command_arrays[pc->current-1];
    assert(compare_string(cmd->commands[0], "g"));
    assert(compare_string(cmd->commands[1], "i"));
    assert(cmd->commands[2] == NULL);
    assert(compare_string(cmd->std_out, "h"));
    assert(cmd->current == 3);

    parse_command(&pc, strdup("j k&l m>n"));
    assert(pc->current == 5);
    assert(pc->length == 8);

    cmd = pc->command_arrays[pc->current-2];
    assert(compare_string(cmd->commands[0], "j"));
    assert(compare_string(cmd->commands[1], "k"));
    assert(cmd->commands[2] == NULL);
    assert(cmd->current == 3);

    cmd = pc->command_arrays[pc->current-1];
    assert(compare_string(cmd->commands[0], "l"));
    assert(compare_string(cmd->commands[1], "m"));
    assert(cmd->commands[2] == NULL);
    assert(cmd->current == 3);

    free_parallel_commands_and_all(&pc);
}

int main() {
    test_parse_command_helper();
    test_parse_operator();
    test_parallel_commands();
}