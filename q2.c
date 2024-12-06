#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RPM 72
#define SEC_PER_TRACK 100
#define SEEK_MS 2

int find_closest(int cur_pos, int* reqs, int req_count, int* proc) {
    int min_dist = __INT_MAX__;
    int closest = -1;

    for (int i = 0; i < req_count; i++) {
        if (!proc[i]) {
            int dist = abs(cur_pos - reqs[i]);
            if (dist < min_dist) {
                min_dist = dist;
                closest = i;
            }
        }
    }
    return closest;
}

void simulate_sstf(int count, int* reqs, int start_pos) {
    int* proc = (int*)malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        proc[i] = 0;
    }

    int cur_pos = start_pos;

    for (int i = 0; i < count; i++) {
        int next_idx = find_closest(cur_pos, reqs, count, proc);
        int next_pos = reqs[next_idx];

        int cur_track = cur_pos / SEC_PER_TRACK;
        int next_track = next_pos / SEC_PER_TRACK;
        int seek_dist = abs(cur_track - next_track);
        int seek_time = seek_dist * SEEK_MS;

        int cur_sec = cur_pos % SEC_PER_TRACK;
        int next_sec = next_pos % SEC_PER_TRACK;
        int sec_diff = abs(next_sec - cur_sec);

        double rot_latency = (sec_diff * 60.0) / (RPM * SEC_PER_TRACK);

        cur_pos = next_pos;
        proc[next_idx] = 1;

        printf("Seek Time %d: %d ms\n", i + 1, seek_time);
        printf("Rotational Latency %d: %.2f ms\n", i + 1, rot_latency);
    }

    free(proc);
}

int main() {
    int count;
    printf("Enter the number of disk requests: ");
    scanf("%d", &count);

    int* reqs = (int*)malloc(count * sizeof(int));
    printf("Enter the requests: ");
    for (int i = 0; i < count; i++) {
        scanf("%d", &reqs[i]);
    }

    int start_pos = 100;
    simulate_sstf(count, reqs, start_pos);

    free(reqs);
    return 0;
}
