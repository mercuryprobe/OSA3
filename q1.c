#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
int forks[5] = {0, 0, 0, 0, 0};
pthread_mutex_t locks[5];

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

    // printf("%d\n", i);
    while (1) {
        if (i<4) {
            pickFork(i);
            pickFork((i+1)%5);
            printf("+P%d eating (forks %d and %d)!\n", i, i, (i+1)%5);
            printf("[%d, %d, %d, %d, %d]\n", forks[0], forks[1], forks[2], forks[3], forks[4]);
            eat(i);
            sleep(1);
            printf("-P%d finish (forks %d and %d)!\n", i, i, (i+1)%5);
            putFork(i);
            putFork((i+1)%5);
            printf("[%d, %d, %d, %d, %d]\n", forks[0], forks[1], forks[2], forks[3], forks[4]);
            think(i);
        } else {
            pickFork((i+1)%5);
            pickFork(i);
            printf("+P%d eating (forks %d and %d)!\n", i, i, (i+1)%5);
            printf("[%d, %d, %d, %d, %d]\n", forks[0], forks[1], forks[2], forks[3], forks[4]);
            eat(i);
            sleep(1);
            printf("-P%d finish (forks %d and %d)!\n", i, i, (i+1)%5);
            printf("[%d, %d, %d, %d, %d]\n", forks[0], forks[1], forks[2], forks[3], forks[4]);
            putFork((i+1)%5);
            putFork(i);
            think(i);
        }
    }
}

int main() {
    pthread_t pids[5];
    for (int i = 0; i<5; i++) {
        pthread_mutex_init(&locks[i], NULL);
    }
    
    
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
