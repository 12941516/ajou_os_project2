#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int shared_data = 0;

void* reader(void* arg) {
    int id = *(int*)arg;
    int last_read = -1;

    for (int i = 0; i < 15; i++) {
        usleep(2000);

        if (shared_data != last_read) {
            printf("[Reader %d] Detected change: %d -> %d\n",
                   id, last_read, shared_data);
            last_read = shared_data;
        }
    }
    return NULL;
}

// race condition writer
void* writer(void* arg) {
    for (int i = 0; i < 10; i++) {
        int temp = shared_data;
        usleep(5000);               // race condition
        shared_data = temp + 1;     // write
        printf("    [Writer] Updated value to %d\n", shared_data);
        usleep(1000);
    }
    return NULL;
}

int main() {
    pthread_t r1, r2, w1;
    int id1 = 1, id2 = 2;

    pthread_create(&r1, NULL, reader, &id1);
    pthread_create(&r2, NULL, reader, &id2);
    pthread_create(&w1, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);

    return 0;
}

