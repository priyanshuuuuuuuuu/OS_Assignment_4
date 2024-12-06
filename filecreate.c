#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[20];
};

void create_student_file() {
    FILE *file = fopen("students.rec", "wb");
    if (!file) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }

    struct Student students[5] = {
        {1, "Kunal"},
        {2, "Daksh"},
        {3, "Tanmay"},
        {4, "Dipanshu"},
        {5, "Priyanshu"}
    };

    fwrite(students, sizeof(struct Student), 5, file);
    fclose(file);
    printf("students.rec created successfully.\n");
}

int main() {
    create_student_file();
    return 0;
}
