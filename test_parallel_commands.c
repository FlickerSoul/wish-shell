#include "parallel_commands.h"
#include "test_utils.h"

void test_parse_command() {
    command_array* cmd = parse_command_(strdup("a b c"));
    assert(compare_string(cmd->commands[0], "a"));
    assert(compare_string(cmd->commands[1], "b"));
    assert(compare_string(cmd->commands[2], "c"));
}

void test_parallel_commands() {
    
}

int main() {
    test_parse_command();
    test_parallel_commands();
}