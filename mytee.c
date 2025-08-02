#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define MAX_FILES 10

int main(int argc, char *argv[]) {
    FILE *files[MAX_FILES];
    int file_count = 0;
    int append = 0;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-a") == 0) {
            append = 1;
        } else {
            if (file_count >= MAX_FILES) {
                fprintf(stderr, "Too many output files (max %d)\n", MAX_FILES);
                return 1;
            }

            files[file_count] = fopen(argv[i], append ? "a" : "w");
            if (!files[file_count]) {
                perror(argv[i]);
                return 1;
            }
            file_count++;
        }
    }

    // Read from stdin and write to stdout + files
    char buffer[MAX_LINE];
    while (fgets(buffer, MAX_LINE, stdin)) {
        fputs(buffer, stdout);
        for (int i = 0; i < file_count; ++i) {
            fputs(buffer, files[i]);
        }
    }

    // Close all files
    for (int i = 0; i < file_count; ++i) {
        fclose(files[i]);
    }

    return 0;
}
