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
    int valid; // 0 = empty, 1 = full
} job_t;

job_t buffer[BUF_SIZE];

void *producer(void *arg) {
    int id = *(int*)arg;
    for (int i = 0; i < PRODUCE_ITER; i++) {
        int slot = rand() % BUF_SIZE;

        buffer[slot].priority = rand() % 3;
        buffer[slot].value = rand() % 1000;
        buffer[slot].iter = i;
        buffer[slot].valid = 1;
        printf("[Producer %d] Put job at slot %d (priority=%d, iter=%d)\n",
               id, slot, buffer[slot].priority, i);

        usleep(10000); // race condition
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < CONSUME_ITER; i++) {
        int slot = rand() % BUF_SIZE;
        if (buffer[slot].valid == 1) {
            printf(">>> [Consumer] Took job from slot %d (priority=%d, value=%d, iter=%d)\n",
                   slot, buffer[slot].priority, buffer[slot].value, buffer[slot].iter);
            buffer[slot].valid = 0; // empty
        }
        
        else {
            printf(">>> [Consumer] WARNING: slot %d was EMPTY!\n", slot);
        }
        usleep(15000); // race condition
    }
    return NULL;
}

int main() {
    pthread_t prod[PRODUCER_COUNT], cons;
    int ids[PRODUCER_COUNT] = {1, 2};

    srand(time(NULL));

    for (int i = 0; i < BUF_SIZE; i++)
    	buffer[i].valid = 0;

    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_create(&prod[i], NULL, producer, &ids[i]);

    pthread_create(&cons, NULL, consumer, NULL);

    for (int i = 0; i < PRODUCER_COUNT; i++)
        pthread_join(prod[i], NULL);
    pthread_join(cons, NULL);

    printf("\n=== NO SYNC VERSION DONE ===\n");
    return 0;
}

