#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
sem_t semaphores[5];
sem_t bowls[2];

static volatile sig_atomic_t active = 1;
static void interrupter(int x) {
    active = 0;
}

void pickFork(int i) {
    sem_wait(&semaphores[i]);
}

void eat(int i) {
    philosopher[i] = 1;
}

void think(int i) {
    philosopher[i] = 0;
}

void putFork(int i) {
    sem_post(&semaphores[i]);
}

int grabSoup(int i) {
    // returns bowl that gets used

    int bowlVals[2];
    (sem_getvalue(&bowls[0], &bowlVals[0]) && (sem_getvalue(&bowls[1], &bowlVals[1])));
    
    sem_wait(&bowls[(bowlVals[0]!=0) && ((bowlVals[1]==0) || (i%2))]); // b0.(b1' + i%2) 
    return ((bowlVals[0]!=0) && ((bowlVals[1]==0) || (i%2))); 

    // if bowl0 was unused at time of sem_getvalue, will attempt to lock bowl0
    // if bowl0 was in use but b1 unused, will attempt to lock bowl1
    // if both used, even philosophers will lock 0, and odd will lock 1
    // --Edge case--
    // if grabSoup gets pre-empted after getValue but before wait, it's possible 
    // that a bowl that was unused when getvalue was run will come to be in use by another philosopher
    // and when it returns to the original grabSoup, the philosopher will have to wait.
    // I've tried my best to ensure that this^ is the only scenario where preemption will cause waiting by condensing all my logic into single statements.
}

void returnSoup(int bowlOccupied) {
    sem_post(&bowls[bowlOccupied]);
}




void *philosphise(void *_i) {
    int i = *((int *) _i);

    while (active) {
        if (i<4) {
            pickFork(i);
            pickFork((i+1)%5);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            int bowlOccupied = grabSoup(i);
            eat(i);
            sleep(1);
            returnSoup(bowlOccupied);
            putFork(i);
            putFork((i+1)%5);
            printf("Philosopher %d think_ [%d::%d]!\n", i, i, (i+1)%5);
            think(i);
        } else {
            pickFork((i+1)%5);
            pickFork(i);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            int bowlOccupied = grabSoup(i);
            eat(i);
            sleep(1);
            returnSoup(bowlOccupied);
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

    for (int i = 0; i<2; i++) {
        sem_init(&bowls[i], 0, 1);
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
