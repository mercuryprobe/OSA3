#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
sem_t semaphores[5];

static volatile sig_atomic_t active = 1;
static void interrupter(int x) {
    active = 0;
}

int pickFork(int i) {
    sem_wait(&semaphores[i]);
}

void eat(int i) {
    philosopher[i] = 1;
}

void think(int i) {
    philosopher[i] = 0;
}


int putFork(int i) {
    sem_post(&semaphores[i]);
}


void *philosphise(void *_i) {
    int i = *((int *) _i);

    while (active) {
        if (i<4) {
            pickFork(i);
            pickFork((i+1)%5);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            eat(i);
            sleep(1);
            putFork(i);
            putFork((i+1)%5);
            printf("Philosopher %d thinkng [%d::%d]!\n", i, i, (i+1)%5);
            think(i);
        } else {
            pickFork((i+1)%5);
            pickFork(i);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            eat(i);
            sleep(1);
            putFork((i+1)%5);
            putFork(i);
            printf("Philosopher %d thinkng [%d::%d]!\n", i, i, (i+1)%5);
            think(i);
        }
    }
}

int main() {
    puts("---Semaphore---");
    pthread_t pids[5];
    for (int i = 0; i<5; i++) {
        sem_init(&semaphores[i], 0, 1);
    }
    
    signal(SIGINT, interrupter);
    for (int i=0; i<5; i++) {
        int *_i = malloc(sizeof(*_i));
        *_i = i;
        printf("thread started: %d\n", i);
        pthread_create(&pids[i], NULL, &philosphise, _i);
    }

    for (int i=0; i<5; i++) {
        pthread_join(pids[i], NULL);
    }

    return 0;
}
