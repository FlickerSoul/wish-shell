#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


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
void exit_(char* const args[]) {

}

/**
 * cd to a directory 
 * use chdir() sys call
 * accept exact one argument
 * raise error when there are 
 * more/less than one argument 
 * or the dir does not exit.
 */
void cd_(char* const args[]) {

}

/**
 * add path to shell path
 */
void path_(char* const args[]) {
    
}

void built_in_command(char** const command, char* const args[]) {
// I also need array length?
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

int main(int argc, char* argv[]) {
    /**
     * special operator 
     * > for stdout/stderr 
     * & for parallel executing 
     * (optional)
     * | piping 
     * < stdin
     * 
     * readline lib 
     * environ 
     */

    if (argc > 1) {
        batch_mode();
    } else {
        interactive_mode();
    }
}