#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int shared_data = 0;
int readcount = 0;

pthread_mutex_t mutex;
pthread_mutex_t rw_mutex;

void* reader(void* arg) {
    int last = -1;
    for (int i = 0; i < 20; i++) {
        usleep(2000);

        // ---Reader Entry---
        pthread_mutex_lock(&mutex);
        readcount++;
        if (readcount == 1)
            pthread_mutex_lock(&rw_mutex);
        pthread_mutex_unlock(&mutex);

        // ---Critical Section---
        if (shared_data != last) {
            printf("[Reader] Detected change: %d -> %d\n", last, shared_data);
            last = shared_data;
        }

        // ---Reader Exit---
        pthread_mutex_lock(&mutex);
        readcount--;
        if (readcount == 0)
            pthread_mutex_unlock(&rw_mutex);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* writer(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&rw_mutex);
        shared_data++;
        printf("    [Writer] Updated value to %d\n", shared_data);
        pthread_mutex_unlock(&rw_mutex);
        usleep(5000);
    }
    return NULL;
}

int main() {
    pthread_t r1, r2, w1;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&rw_mutex, NULL);

    pthread_create(&r1, NULL, reader, NULL);
    pthread_create(&r2, NULL, reader, NULL);
    pthread_create(&w1, NULL, writer, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(w1, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&rw_mutex);

    return 0;
}

