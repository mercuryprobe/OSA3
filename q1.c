#include <pthread.h>
#include <unistd.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
int forks[5] = {0, 0, 0, 0, 0};
pthread_mutex_t locks[5];

int pickFork(int i) {
    pthread_mutex_lock(&locks[i]);
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

    pthread_mutex_unlock(&lock[i]);
    return result;
}


void *philosphise(void *_i) {
    int i = (int) _i;
    while (1) {
        if (i<4) {
            pickFork(i);
            pickFork((i+1)%5);
            eat(i);
            sleep(1);
            putFork((i+1)%5);
            putFork(i);
            think(i);
        } else {
            pickFork((i+1)%5);
            pickFork(i);
            eat(i);
            sleep(1);
            putFork(i);
            putFork((i+1)%5);
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
        pthread_create(&pids[i], NULL, &philosphise, (void *) i);
    }

    return 0;
}
