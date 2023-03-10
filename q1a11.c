#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
int forks[5] = {0, 0, 0, 0, 0};
pthread_mutex_t locks[5];

static volatile sig_atomic_t active = 1;
static void interrupter(int x) {
    active = 0;
}

int pickFork(int i) {
    // printf("Locking %d | Status: %d\n", i, pthread_mutex_trylock(&locks[i]));
    pthread_mutex_lock(&locks[i]);
    // printf("----Fork %d picked----\n", i);
    if (forks[i]!=1) {
        forks[i] = 1;
        return 0;
    } else {
        return -1;
    }
}

void eat(int i) {
    philosopher[i] = 1;
}

void think(int i) {
    philosopher[i] = 0;
}


int putFork(int i) {
    int result;
    if (forks[i]!=0) {
        forks[i] = 0;
        result = 0;
    } else {
        result = -1;
    }

    // printf("----Fork %d released----\n", i);
    pthread_mutex_unlock(&locks[i]);
    return result;
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
            printf("Philosopher %d think_ [%d::%d]!\n", i, i, (i+1)%5);
            think(i);
        } else {
            pickFork((i+1)%5);
            pickFork(i);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            eat(i);
            sleep(1);
            putFork((i+1)%5);
            putFork(i);
            printf("Philosopher %d think_ [%d::%d]!\n", i, i, (i+1)%5);
            think(i);
        }
    }
}

int main() {
    pthread_t pids[5];
    for (int i = 0; i<5; i++) {
        pthread_mutex_init(&locks[i], NULL);
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
}
