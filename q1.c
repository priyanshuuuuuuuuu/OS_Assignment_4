#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct {
    char* name;
    double min_temp, max_temp, sum_temp;
    int count;
} StationStats;

void update_station(StationStats* station, double temp) {
    if (temp < station->min_temp) station->min_temp = temp;
    if (temp > station->max_temp) station->max_temp = temp;
    station->sum_temp += temp;
    station->count++;
}

int modify_station(StationStats** stations, int* count, int* capacity, const char* name, double temp) {
    for (int i = 0; i < *count; i++) {
        if (strcmp((*stations)[i].name, name) == 0) {
            update_station(&((*stations)[i]), temp);
            return 0;
        }
    }

    if (*count >= *capacity) {
        *capacity *= 2;
        *stations = realloc(*stations, *capacity * sizeof(StationStats));
        if (!(*stations)) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }

    (*stations)[*count].name = strdup(name);
    (*stations)[*count].min_temp = temp;
    (*stations)[*count].max_temp = temp;
    (*stations)[*count].sum_temp = temp;
    (*stations)[*count].count = 1;
    (*count)++;
    return 1;
}

void print_statistics(StationStats* stations, int count) {
    for (int i = 0; i < count; i++) {
        double mean_temp = stations[i].sum_temp / stations[i].count;
        printf("%s min=%.2f mean=%.2f max=%.2f\n", stations[i].name, stations[i].min_temp, mean_temp, stations[i].max_temp);
        free(stations[i].name);
    }
    free(stations);
}

int process_with_mmap(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("Failed to get file size");
        close(fd);
        return 1;
    }

    char* data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return 1;
    }
    close(fd);

    int capacity = 1000, station_count = 0;
    StationStats* stations = malloc(capacity * sizeof(StationStats));
    if (!stations) {
        perror("Memory allocation failed");
        munmap(data, st.st_size);
        return 1;
    }

    size_t index = 0;
    while (index < st.st_size) {
        size_t line_len = 0;
        while (index + line_len < st.st_size && data[index + line_len] != '\n') {
            line_len++;
        }

        char* line = strndup(data + index, line_len);
        index += line_len + 1;

        char name[1000];
        double temp;
        sscanf(line, "%[^;];%lf", name, &temp);

        modify_station(&stations, &station_count, &capacity, name, temp);
        free(line);
    }

    munmap(data, st.st_size);
    print_statistics(stations, station_count);
    return 0;
}

int process_with_fread(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    int capacity = 1000, station_count = 0;
    StationStats* stations = malloc(capacity * sizeof(StationStats));
    if (!stations) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char name[1000];
        double temp;
        sscanf(line, "%[^;];%lf", name, &temp);
        modify_station(&stations, &station_count, &capacity, name, temp);
    }

    fclose(file);
    print_statistics(stations, station_count);
    return 0;
}

int main() {
    const char* filename = "q1-10mil.txt";

    printf("Processing using mmap:\n");
    if (process_with_mmap(filename)) {
        fprintf(stderr, "mmap processing failed.\n");
    }

    printf("\nProcessing using fread:\n");
    if (process_with_fread(filename)) {
        fprintf(stderr, "fread processing failed.\n");
    }

    return 0;
}



