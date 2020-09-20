#include "wish_utils.h"

wish_state* shell_state = NULL;

void prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input) {
    printf("wish> ");
    char* new_line = NULL;
    getline(&new_line, size, input);
    
    int len = strlen(new_line);

    if (len == 1) {
        free(new_line);
        new_line = NULL;
    } else {
        new_line[len - 1] = '\0';
    }
    
    *input_string_buffer_ptr = new_line;
}

/**
 * print shell err
 */
void print_err() {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

/**
 * exit the shell
 */
void exit_() {
    exit(0);
}

/**
 * cd to a directory 
 * use chdir() sys call
 * accept exact one argument
 * raise error when there are 
 * more/less than one argument 
 * or the dir does not exit.
 */
void cd_(command_array* cmd) {

}

/**
 * add path to shell path
 */
void path_(command_array* cmd) {
    
}

const char* built_in_commands[] = {"exit", "cd",  "path"};
unsigned short built_in_command_num = 3;

bool is_built_in_command(command_array* cmd) {
    char* cmd_str = cmd->commands[0];
    for (int i = 0; i < built_in_command_num; i++) {
        if (!strcmp(cmd_str, built_in_commands[i])) {
            return true;
        }
    }

    return false;
}

void exec_built_in_command(command_array* cmd) {
    char* cmd_str = cmd->commands[0];
    if (!strcmp(cmd_str, "exit")) {
        exit_();
    } else if (!strcmp(cmd_str, "cd")) {
        cd_(cmd);
    } else if (!strcmp(cmd_str, "path")) {
        path_(cmd);
    }
}

void exec_command(command_array* cmd) {

}

void redirect_stdout() {

}

void execute(parallel_commands* pc) {
    // get the path var 
    // use access to find command 
    for (int i = 0; i < pc->current; i++) {
        command_array* cmd = pc->command_arrays[i];

        if (cmd->commands[0] == NULL) {
            continue;
        }
        if (is_built_in_command(cmd)) {
            exec_built_in_command(cmd);
        } else {
            exec_command(cmd);
        }
    }
}

void init_wish_state() {
    shell_state = malloc(sizeof(wish_state));
    if (shell_state == NULL) {
        return;
    }

    shell_state->path = "/bin:/usr/bin";
}

void destroy_wish_state() {
    if (shell_state == NULL) {
        return;
    }
    free(shell_state);
}

/**
 * run batch mode by specifying 
 * a batch file path
 */
void batch_mode() {

}

void interactive_mode() {
    // TODO 
    // Open file 
    // Close file 
    // read line by line? 
    // exec command by command 
    if (shell_state == NULL) {
        perror("Cannot init shell. Exiting.\n");
        exit(1);
    }

    char* new_line = NULL;
    size_t line_size = 0;
    while(1) {
        prompt_input(&new_line, &line_size, stdin);
        parallel_commands* pc = quick_new_parallel_commands();
        parse_command(&pc, new_line);
        execute(pc);
        free_parallel_commands_and_all(&pc);
    }
}