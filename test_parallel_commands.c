#include "parallel_commands.h"
#include "test_utils.h"

void test_parse_command_helper() {
    command_array* cmd = parse_command_(strdup("a b c"));
    assert(compare_string(cmd->commands[0], "a"));
    assert(compare_string(cmd->commands[1], "b"));
    assert(compare_string(cmd->commands[2], "c"));
}

void test_parallel_commands() {
    parallel_commands* pc = quick_new_parallel_commands();
    pc = new_parallel_commands(1, 0);
    assert(pc->current == 0);
    assert(pc->length == 1);

    parse_command(&pc, strdup("a b c"));
    assert(pc->current == 1);
    assert(pc->length == 2);

    command_array* cmd = pc->command_arrays[0];
    assert(compare_string(cmd->commands[0], "a"));
    assert(compare_string(cmd->commands[1], "b"));
    assert(compare_string(cmd->commands[2], "c"));
    assert(cmd->commands[3] == NULL);
    assert(cmd->current == 4);

    parse_command(&pc, strdup("   d        e f     "));
    assert(pc->current == 2);
    assert(pc->length == 4);

    cmd = pc->command_arrays[1];
    assert(compare_string(cmd->commands[0], "d"));
    assert(compare_string(cmd->commands[1], "e"));
    assert(compare_string(cmd->commands[2], "f"));
    assert(cmd->commands[3] == NULL);
    assert(cmd->current == 4);
}

int main() {
    test_parse_command_helper();
    test_parallel_commands();
    test_parallel_commands();
}