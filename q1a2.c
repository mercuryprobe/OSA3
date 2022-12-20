#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
sem_t semaphores[5];
int l3[3] = {-1, -1, -1};
int maintainer=0;

static volatile sig_atomic_t active = 1;
static void interrupter(int x) {
    active = 0;
}

void pickFork(int i) {
    sem_wait(&semaphores[i]);
}

void eat(int i) {
    philosopher[i] = 1;
    int curMain = maintainer++;
    l3[curMain%3]=i;
}

void think(int i) {
    philosopher[i] = 0;
}


void putFork(int i) {
    sem_post(&semaphores[i]);
}

int l3Check(int j) {
    // checks if more than 1 of the last two eatings were of the same philosopher
    int num;
    for (int i =0; i<3; i++) {
        if (j==l3[i]) {num++;}
    }
    return -(num>1); 
}

void *philosphise(void *_i) {
    int i = *((int *) _i);

    while (active) {
        int outp;
        if (outp = (l3Check(i)==-1)) {usleep(200);}
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
