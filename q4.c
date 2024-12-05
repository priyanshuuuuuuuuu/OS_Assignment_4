#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void part1(const char *output_file) {
    // Open the file for writing
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening file for part1");
        exit(EXIT_FAILURE);
    }

    // Duplicate the current stdout
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdout < 0) {
        perror("Error duplicating stdout");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Flush stdout and redirect to the file
    fflush(stdout);
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("Error redirecting stdout for part1");
        close(fd);
        close(original_stdout);
        exit(EXIT_FAILURE);
    }

    // Write some random text
    printf("This is some random text written to %s\n", output_file);

    // Restore original stdout
    fflush(stdout);
    if (dup2(original_stdout, STDOUT_FILENO) < 0) {
        perror("Error restoring stdout");
    }

    // Close descriptors
    close(original_stdout);
    close(fd);
}

void part2(const char *command, const char *output_file) {
    // Open the file for writing
    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error opening file for part2");
        exit(EXIT_FAILURE);
    }

    // Duplicate stdout and stderr
    int original_stdout = dup(STDOUT_FILENO);
    int original_stderr = dup(STDERR_FILENO);
    if (original_stdout < 0 || original_stderr < 0) {
        perror("Error duplicating stdout/stderr");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Flush and redirect stdout and stderr to the file
    fflush(stdout);
    fflush(stderr);
    if (dup2(fd, STDOUT_FILENO) < 0 || dup2(fd, STDERR_FILENO) < 0) {
        perror("Error redirecting stdout/stderr for part2");
        close(fd);
        close(original_stdout);
        close(original_stderr);
        exit(EXIT_FAILURE);
    }

    // Execute the command
    int ret = system(command);
    if (ret == -1) {
        perror("Error executing command");
    }

    // Restore original stdout and stderr
    fflush(stdout);
    fflush(stderr);
    if (dup2(original_stdout, STDOUT_FILENO) < 0 || 
        dup2(original_stderr, STDERR_FILENO) < 0) {
        perror("Error restoring stdout/stderr");
    }

    // Close descriptors
    close(original_stdout);
    close(original_stderr);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "%s -p1 <output_file>\n", argv[0]);
        fprintf(stderr, "%s -p2 \"<command>\" <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "-p1") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s -p1 <output_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
        part1(argv[2]);
    } else if (strcmp(argv[1], "-p2") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Usage: %s -p2 \"<command>\" <output_file>\n", argv[0]);
            return EXIT_FAILURE;
        }
        part2(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Invalid option. Use -p1 or -p2.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
