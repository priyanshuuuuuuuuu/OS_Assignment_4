#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct Student {
    int id;
    char name[20];
};

void retrieve_student_by_id(int id) {
    int fd = open("students.rec", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    off_t offset = (id - 1) * sizeof(struct Student);
    if (lseek(fd, offset, SEEK_SET) < 0) {
        perror("Error seeking file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    struct Student student;
    if (read(fd, &student, sizeof(struct Student)) != sizeof(struct Student)) {
        perror("Error reading file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Student name: %s\n", student.name);

    close(fd);
}

int main() {
    int id;
    printf("Enter student ID (1-5): ");
    scanf("%d", &id);

    if (id < 1 || id > 5) {
        printf("Invalid ID. Please enter a value between 1 and 5.\n");
        return EXIT_FAILURE;
    }

    retrieve_student_by_id(id);
    return 0;
}
