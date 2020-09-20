#include "commands.h"
#include "test_utils.h"

void test_commands() {
    command_array* c = quick_new_command_arr();
    free_all_commands_and_arr(&c);
    c = new_command_arr(1, 0, NULL);
    assert(c != NULL);
    assert(c->length == 1);
    assert(c->current == 0);

    char* dummy_command = strdup("command");
    char* dummy_command2 = strdup("command2");
    push_command(&c, dummy_command);
    assert(compare_string(dummy_command, c->commands[0]));
    assert(c->length == 2);
    assert(c->current == 1);

    char* dummy_out_name = strdup("std_out");
    put_std_out(c, dummy_out_name);
    assert(compare_string(c->std_out, dummy_out_name));

    push_command(&c, dummy_command2);
    wrap_up_command(&c);
    assert(compare_string(dummy_command2, c->commands[1]));
    assert(c->commands[2] == NULL);
    assert(c->length == 4);
    assert(c->current == 3);

    assert(compare_string(c->std_out, dummy_out_name));
    
    free_all_commands_and_arr(&c);
    assert(c == NULL);
}

int main() {
    test_commands();
}