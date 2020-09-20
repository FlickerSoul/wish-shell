#include "wish_utils.h"

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

char* built_in_commands = {"exit", "cd",  "path"};
unsigned short built_in_command_num = 3;

bool is_built_in_command(command_array* cmd) {
    char* cmd_str = cmd->commands[0];
    for (int i = 0; i < built_in_command_num; i++) {
        if (strcmp(cmd_str, built_in_commands[i]) == 0) {
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
    } else if (!strcmp(cmd_str), "path") {
        path_(cmd);
    }
}

void redirect_stdout() {

}

void execute(char* command) {
    // get the path var 
    // use access to find command 
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