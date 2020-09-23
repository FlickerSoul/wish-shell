#include "wish_utils.h"

wish_state* shell_state = NULL;
char cwd[PATH_MAX];


int prompt_input(char** input_string_buffer_ptr, size_t* size, FILE* input) {
    char* new_line = NULL;
    int end = getline(&new_line, size, input);
    
    int len = strlen(new_line);

    new_line[len - 1] = '\0';
    
    *input_string_buffer_ptr = new_line;
    return end;
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
void exit_(command_array* cmd) {
    int cmd_num = cmd->current - 1;
    if (cmd_num > 1) {
        // print_err();
        perror("exit takes not args");
        return;
    }
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

    int cmd_num = cmd->current - 1;
    if (cmd_num == 1 || cmd_num > 2) {
        // print_err();
        perror("cd takes only one command");
        return;
    }

    int result = chdir(path);
    if (result != 0) {
        // print_err();
        perror("cannot cd to path");
    }

    // printf("%s\n", getcwd(cwd, sizeof(cwd)));
}

void add_shell_path(char* path) {
    if (shell_state->current == shell_state->size) {
        expand_path();
    }

    char* temp = NULL;
    if (path[0] != '/') {
        getcwd(cwd, PATH_MAX);
        temp = malloc((strlen(path) + strlen(cwd)) * sizeof(char) + 2);
        strcpy(temp, cwd);
        strcat(temp, "/");
        strcat(temp, path);
    } else {
        temp = strdup(path);
    }

    shell_state->path[shell_state->current++] = temp;
}

void replace_path() {
    char** new_paths = malloc(sizeof(char*));
    for (int i = 0; i < shell_state->current; i++) {
        free(shell_state->path[i]);
    }
    free(shell_state->path);

    shell_state->current = 0;
    shell_state->size = 1;
    shell_state->path = new_paths;
}

/**
 * add path to shell path
 */
void path_(command_array* cmd) {
    struct stat sb;
    char* path = NULL;
    replace_path();
    for (int i = 1; i < cmd->current - 1; i++) {
        path = cmd->commands[i];
        add_shell_path(path);
    }
    // printf("PATH: %s\n", shell_state->path);
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
        exit_(cmd);
    } else if (!strcmp(cmd_str, "cd")) {
        cd_(cmd);
    } else if (!strcmp(cmd_str, "path")) {
        path_(cmd);
    }
}

bool find_cmd(command_array* cmd) {
    char** paths = shell_state->path;
    char* sep = NULL;

    for (int i = 0; i < shell_state->current; i++) {
        char* sep = malloc((strlen(paths[i]) + strlen(cmd->commands[0])) * sizeof(char) + 2);
        strcpy(sep, paths[i]);
        strcat(sep, "/");
        strcat(sep, cmd->commands[0]);

        if (access(sep,X_OK) == 0) {
            free(cmd->commands[0]);
            cmd->commands[0] = sep;
            return true;
        } else {
            free(sep);
        }

        sep = NULL;
    }
    return false;
}

FILE* redirect_stdout(char* path) {
    if (path != NULL) {
        FILE* file = fopen(path, "w");

        if (file == NULL) {
            perror("cannot open file");
        }
        int result = dup2(fileno(file), STDOUT_FILENO);
        if (result == -1) {
            perror("cannot redirect stdout to a file");
        }

        result = dup2(fileno(file), STDERR_FILENO);
        if (result == -1) {
            perror("cannot redirect stderr to a file");
        }
        return file;
    }
    return NULL;
}

FILE* redirect_stdin(char* path) {
    if (path != NULL) {
        FILE* file = fopen(path, "r");

        if (file == NULL) {
            perror("cannot open file");
        }

        int result = dup2(fileno(file), STDIN_FILENO);
        if (result == -1) {
            perror("cannot redirect stdin from a file");
        }

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
            FILE* std_in_redir = redirect_stdin(cmd->std_in);
            execv(cmd->commands[0], cmd->commands);
            fclose(std_out_redir);
            fclose(std_in_redir);
            // print_err();
            perror("cannot exec command");
        } 
        // print_err();
        perror("cannot find command");
    }

    return new_pid;
}

bool non_empty_cmd(command_array* cmd) {
    return cmd->std_out != NULL;
}

void execute(parallel_commands* pc) {
    // get the path var 
    // use access to find command 
    pid_t pid_arr[pc->current];
    int pid_counter = 0;

    for (int i = 0; i < pc->current; i++) {
        command_array* cmd = pc->command_arrays[i];

        if (cmd->commands[0] == NULL) {
            if (non_empty_cmd(cmd)) {
                // print_err();
                perror("empty command");
            }
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
    shell_state->size = 1;
    shell_state->current = 1;
    shell_state->path = malloc(sizeof(char*));
    shell_state->path[0] = strdup("/bin");
}

void expand_path() {
    if (shell_state == NULL) {
        return;
    }

    shell_state->size *= 2;

    char** new_paths = malloc(shell_state->size * sizeof(char*));
    for (int i = 0; i < shell_state->current; i++) {
        new_paths[i] = shell_state->path[i];
    }
    free(shell_state->path);
    shell_state->path = new_paths;
}

void destroy_wish_state() {
    if (shell_state == NULL) {
        return;
    }
    for (int i = 0; i < shell_state->current; i++) {
        free(shell_state->path[i]);
    }
    free(shell_state->path);
    free(shell_state);
    shell_state = NULL;
}

/**
 * run batch mode by specifying 
 * a batch file path
 */
void batch_mode(int argc, char** filenames) {
    if (argc > 2) {
        // print_err();
        perror("too many args");
        exit(1);
    }

    char* filename = filenames[1];
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // print_err();
        perror("cannot open command list file");
        exit(1);
    }
    char* new_line = NULL;
    size_t size = 0;
    int end = prompt_input(&new_line, &size, file);
    // printf("end line: %i\n", end);

    while(new_line != NULL && strcmp(new_line, "") != 0 && end != -1) {
        parallel_commands* pc = quick_new_parallel_commands();
        bool success = parse_command(&pc, new_line);
        if (success) {
            execute(pc);
        } else {
            // print_err();
            perror("cannot parse command");
        }
        free_parallel_commands_and_all(&pc);
        prompt_input(&new_line, &size, file);
    } 
    fclose(file);
}

void interactive_mode() {
    if (shell_state == NULL) {
        perror("Cannot init shell. Exiting.\n");
        exit(1);
    }

    char* new_line = NULL;
    size_t line_size = 0;
    while(1) {
        printf("wish> ");
        prompt_input(&new_line, &line_size, stdin);

        if (!strcmp(new_line, "")) {
            continue;
        }

        parallel_commands* pc = quick_new_parallel_commands();
        bool success = parse_command(&pc, new_line);
        if (success) {
            execute(pc);
        } else {
            // print_err();
            perror("cannot parse command");
        }
        free_parallel_commands_and_all(&pc);
    }
}