#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int run_cmd(const char *cmd) {
    printf("\n[RUN] %s\n", cmd);
    int status = system(cmd);
    if (status != 0) {
        fprintf(stderr, "Error: Command failed with exit code %d\n", status);
        return 0;
    }
    return 1;
}

void apply_compiler(char *dest, size_t dest_size, const char *orig_cmd, const char *new_cc) {
    if (!new_cc || strlen(new_cc) == 0) {
        snprintf(dest, dest_size, "%s", orig_cmd);
        return;
    }

    const char *space = strchr(orig_cmd, ' ');
    if (space) {
        snprintf(dest, dest_size, "%s%s", new_cc, space);
    } else {
        snprintf(dest, dest_size, "%s", new_cc);
    }
}

int main(int argc, char **argv) {
    const char *target_cc = NULL;
    int target_index = -1; // -1 means run all commands by default

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--cc=", 5) == 0) {
            target_cc = argv[i] + 5;
            size_t len = strlen(target_cc);
            if (len >= 2 && target_cc[0] == '"' && target_cc[len - 1] == '"') {
                char *temp = malloc(len - 1);
                strncpy(temp, target_cc + 1, len - 2);
                temp[len - 2] = '\0';
                target_cc = temp;
            }
        } else if (strcmp(argv[i], "--i") == 0 && i + 1 < argc) {
            target_index = atoi(argv[i + 1]);
            i++;
        }
    }

    const char *commands[] = {
        "g++ exp_eval.cpp -o expEval",                                      // Index 0
        "clang++ -std=c++17 -Wall -Wextra -ggdb -o benchmark_io.exe benchmark_io.cpp", // Index 1
        "clang++ -std=c++17 -Wall -Wextra -ggdb codebaseAnalyzer.cpp -o codebaseAnalyzer.exe", // Index 2
        "g++ -o percep perceptron.cpp",                                     // Index 3
        "g++ -o mytool mytooldoc.cpp",                                      // Index 4
        "clang++ -Wall -Wextra -ggdb levenshtein.cpp -o levenshtein"          // Index 5
    };

    int n = sizeof(commands) / sizeof(commands[0]);

    if (target_index >= 0) {
        if (target_index >= n) {
            fprintf(stderr, "Error: Index %d out of range (0 to %d)\n", target_index, n - 1);
            return 1;
        }
        char modified_cmd[512];
        apply_compiler(modified_cmd, sizeof(modified_cmd), commands[target_index], target_cc);
        if (!run_cmd(modified_cmd)) {
            return 1;
        }
    } else {
        for (int i = 0; i < n; i++) {
            char modified_cmd[512];
            apply_compiler(modified_cmd, sizeof(modified_cmd), commands[i], target_cc);
            if (!run_cmd(modified_cmd)) {
                fprintf(stderr, "Build aborted due to errors.\n");
                return 1;
            }
        }
    }

    printf("\nExecution completed successfully!\n");
    return 0;
}


// Usage Examples
// Compile the builder once:

// Bash
// gcc builder.c -o builder
// Run a single command at index 2 using gcc:

// Bash
// ./builder --cc="gcc" --i 2
// Run all commands using clang:

// Bash
// ./builder --cc="clang"
// Run all commands using their default configured compilers (omitting --cc):

// Bash
// ./builder
