#include "commands.h"
#include "test_utils.h"

void test_commands() {
    command_array* c = quick_new_command_arr();
    c = new_command_arr(1, 0);
    assert(c != NULL);
    assert(c->length == 1);
    assert(c->current == 0);

    char* dummy_command = "command";
    push_command(&c, strdup(dummy_command));
    assert(compare_string(dummy_command, c->commands[0]));
    assert(c->length == 2);
    assert(c->current == 1);

    push_command(&c, strdup(dummy_command));
    assert(compare_string(dummy_command, c->commands[1]));
    assert(c->length == 4);
    assert(c->current == 2);

    free_all_commands_and_arr(&c);
    assert(c == NULL);
}

int main() {
    test_commands();
}