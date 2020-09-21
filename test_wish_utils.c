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
    command_array* cmd = parse_simple_command_helper(strdup("path /bin /usr/bin"));
    init_wish_state();
    assert(shell_state->current == 1);
    assert(compare_string(shell_state->path[0], "/bin"));

    replace_path();
    assert(shell_state->current == 0);

    path_(cmd);
    assert(shell_state->current == 2);

    assert(compare_string(shell_state->path[0], "/bin"));
    assert(compare_string(shell_state->path[1], "/usr/bin"));

    command_array* cmd2 = parse_simple_command_helper(strdup("path tests"));

    path_(cmd2);
    assert(shell_state->current == 1);
    
    char cwd[256];
    char* temp = malloc((strlen("tests") + strlen(cwd)) * sizeof(char));
    strcat(temp, cwd);
    strcat(temp, "/");
    strcat(temp, "tests");
    assert(compare_string(temp, shell_state->path[0]));

    free(temp);

    destroy_wish_state();
    assert(shell_state == NULL);
    free_all_commands_and_arr(&cmd);
    free_all_commands_and_arr(&cmd2);
}

void test_find_command() {
    init_wish_state();
    command_array* cmd = parse_simple_command_helper(strdup("ls"));
    assert(find_cmd(cmd));
    assert(compare_string("/bin/ls", cmd->commands[0]));
    
    command_array* cmd2 = parse_simple_command_helper(strdup("noexistscmd"));
    assert(!find_cmd(cmd2));
    assert(compare_string("noexistscmd", cmd2->commands[0]));
    destroy_wish_state();
    free_all_commands_and_arr(&cmd);
    free_all_commands_and_arr(&cmd2);
}

void test_execommand() {
    init_wish_state();
    command_array* cmd = parse_simple_command_helper(strdup("ls"));
    cmd->std_out = strdup("a.txt");
    pid_t pid = exec_command(cmd);
    if (pid == 0) {
        exit(1);
    } 
    int status = 0;
    waitpid(pid, &status, 0);
    destroy_wish_state();
    free_all_commands_and_arr(&cmd);
}

int main() {
    test_prompt_input();
    test_execommand();
    test_cd();
    test_path();
}