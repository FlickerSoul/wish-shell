#include "wish_utils.h"

wish_state* shell_state = NULL;
char cwd[PATH_MAX];


void prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input) {
    printf("wish> ");
    char* new_line = NULL;
    getline(&new_line, size, input);
    
    int len = strlen(new_line);

    new_line[len - 1] = '\0';
    
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
    char* path = cmd->commands[1];
    int result = chdir(path);
    if (result != 0) {
        perror("cannot cd to path");
        // print_err();
    }

    printf("%s\n", getcwd(cwd, sizeof(cwd)));
}

void add_shell_path(char* path) {
    strcat(shell_state->path, path);
}

/**
 * add path to shell path
 */
void path_(command_array* cmd) {
    struct stat sb;
    char* path = NULL;
    for (int i = 1; i < cmd->current - 1; i++) {
        path = cmd->commands[i];
        if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            add_shell_path(":");
            add_shell_path(path);
        } else {
            perror("adding non-dir to path");
        }
    }
    printf("PATH: %s\n", shell_state->path);
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

bool find_cmd(command_array* cmd) {
    char* last = shell_state->path;
    char* sep = NULL;

    while ((sep = strtok_r(last, ":", &last)) != NULL) {
        sep = strdup(sep);
        strcat(sep, "/");
        strcat(sep, cmd->commands[0]);
        if (access(sep,X_OK) == 0) {
            free(cmd->commands[0]);
            cmd->commands[0] = sep;
            return true;
        } else {
            free(sep);
        }
    }
    return false;
}

FILE* redirect_stdout(char* path) {
    if (path != NULL) {
        FILE* file = fopen(path, "w");

        if (file == NULL) {
            perror("cannot open file");
        }
        dup2(fileno(file), STDOUT_FILENO);
        return file;
    }
    return NULL;
}

pid_t exec_command(command_array* cmd) {
    pid_t new_pid = fork();
    if (new_pid == -1) {
        perror("cannot fork new process");
    } else if (new_pid == 0) {
        if (find_cmd(cmd)) {
            FILE* std_out_redir = redirect_stdout(cmd->std_out);
            execv(cmd->commands[0], cmd->commands);
            fclose(std_out_redir);
            perror("cannot exec command");
        } 
        perror("cannot find command");
        // print_err();
    }

    return new_pid;
}

void execute(parallel_commands* pc) {
    // get the path var 
    // use access to find command 
    pid_t pid_arr[pc->current];
    int pid_counter = 0;
    for (int i = 0; i < pc->current; i++) {
        command_array* cmd = pc->command_arrays[i];

        if (cmd->commands[0] == NULL) {
            continue;
        }
        if (is_built_in_command(cmd)) {
            exec_built_in_command(cmd);
        } else {
            pid_t pid = exec_command(cmd);
            if (pid == 0) {
                exit(1);
            } else if (pid != -1) {
                pid_arr[pid_counter++] = pid;
            }
        }
    }

    for (int i = 0; i < pid_counter; i++) {
        int status = 0;
        waitpid(pid_arr[i], &status, 0);
    }
}

void init_wish_state() {
    shell_state = malloc(sizeof(wish_state));
    if (shell_state == NULL) {
        return;
    }

    shell_state->path = strdup("/bin:/usr/bin");
}

void destroy_wish_state() {
    if (shell_state == NULL) {
        return;
    }
    free(shell_state->path);
    free(shell_state);
    shell_state = NULL;
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