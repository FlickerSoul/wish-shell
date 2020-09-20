#include "wish_utils.h"

wish_state* state;

void prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input) {
    printf("wish> ");
    getline(input_string_buffer_ptr, size, input);
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

void execute(command_array* cmd) {
    // get the path var 
    // use access to find command 

    if (cmd->commands[0] == NULL) {
        return;
    }

    if (is_built_in_command(cmd)) {
        exec_built_in_command(cmd);
    }
}

wish_state* init_wish_state() {
    state = malloc(sizeof(wish_state));
    if (state == NULL) {
        return NULL;
    }

    state->path = "/bin:/usr/bin";
    return state;
}

void destroy_wish_state() {
    if (state == NULL) {
        return;
    }
    free(state);
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
}