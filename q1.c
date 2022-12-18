#include <pthread.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
int fork[5] = {0, 0, 0, 0, 0};
pthread_mutex_t locks[5];

for (int i = 0; i<5; i++ ) {
    pthread_mutex_init(&locks[i], NULL);
}

int pickFork(i) {
    pthread_mutex_lock(&locks[i]);
    if (fork[i]!=1) {
        fork[i] = 1;
        return 0;
    } else {
        return -1;
    }
}

void eat(i) {
    philosopher[i] = 1;
}

void think(i) {
    philosopher[i] = 0;
}


int putFork(i) {
    int result;
    if (fork[i]!=0) {
        fork[i] = 0;
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
    
    for (int i=0; i<5; i++) {
        pthread_create(&pids[i], NULL, &philosphise, (void *) i);
    }

    return 0;
}
