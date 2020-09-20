#include "wish_utils.h"
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

void test_prompt_input() {
    char* buffer = NULL;
    size_t size = 0;
    prompt_input(&buffer, &size, stdin);
    assert(compare_string(buffer, "test"));
    free(buffer);

    prompt_input(&buffer, &size, stdin);
    assert(compare_string(buffer, "ls > a"));
    free(buffer);

    prompt_input(&buffer, &size, stdin);
    assert(compare_string(buffer, ""));
    free(buffer);
}

void test_cd() {
    command_array* cmd = parse_simple_command_helper(strdup("cd /bin"));
    cd_(cmd);
    char cwd[256];
    assert(compare_string("/bin", getcwd(cwd, sizeof(cwd))));
    free_all_commands_and_arr(&cmd);
}

void test_path() {
    command_array* cmd = parse_simple_command_helper(strdup("path /bin"));
    init_wish_state();
    assert(compare_string(shell_state->path, "/bin:/usr/bin"));
    path_(cmd);
    assert(compare_string(shell_state->path, "/bin:/usr/bin:/bin"));
    destroy_wish_state();
    assert(shell_state == NULL);
    free_all_commands_and_arr(&cmd);
}

int main() {
    test_prompt_input();
    test_cd();
    test_path();
}