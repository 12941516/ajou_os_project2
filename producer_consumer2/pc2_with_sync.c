#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUF_SIZE 5
#define PRODUCER_COUNT 2
#define PRODUCE_ITER 20
#define CONSUME_ITER (PRODUCER_COUNT * PRODUCE_ITER)

typedef struct {
    int priority;
    int value;
    int iter;
} job_t;

job_t buffer[BUF_SIZE];
int count = 0;
int in = 0, out = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    int id = *(int*)arg;
    for (int i = 0; i < PRODUCE_ITER; i++) {
        pthread_mutex_lock(&m);

        while (count == BUF_SIZE)
            pthread_cond_wait(&full, &m);

        buffer[in].priority = rand() % 3;
        buffer[in].value = rand() % 1000;
        buffer[in].iter = i;
        printf("[Producer %d] Put job at slot %d (priority=%d, iter=%d)\n", id, in, buffer[in].priority, i);

        in = (in + 1) % BUF_SIZE;
        count++;

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&m);

        usleep(10000);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < CONSUME_ITER; i++) {
        pthread_mutex_lock(&m);

        while (count == 0)
            pthread_cond_wait(&empty, &m);

        printf(">>> [Consumer] Took job from slot %d (priority=%d, value=%d, iter=%d)\n",
               out, buffer[out].priority, buffer[out].value, buffer[out].iter);

        out = (out + 1) % BUF_SIZE;
        count--;

        pthread_cond_signal(&full);
        pthread_mutex_unlock(&m);

        usleep(15000);
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCER_COUNT], cons;
    int ids[PRODUCER_COUNT] = {1, 2};

    srand(time(NULL));

    for (int i = 0; i < BUF_SIZE; i++)
        buffer[i].priority = buffer[i].value = buffer[i].iter = 0;

    // Producers
    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_create(&prod[i], NULL, producer, &ids[i]);

    // Consumer
    pthread_create(&cons, NULL, consumer, NULL);

    // Join
    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_join(prod[i], NULL);
    pthread_join(cons, NULL);

    printf("\n=== SYNC VERSION DONE ===\n");
    return 0;
}

