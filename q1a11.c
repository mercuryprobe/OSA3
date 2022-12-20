#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

int philosopher[5] = {0, 0, 0, 0, 0}; //default: thinking
int forks[5] = {-1, -1, -1, -1, -1};

static volatile sig_atomic_t active = 1;
static void interrupter(int x) {
    active = 0;
}

int pickFork(int i, int philNum) {
    if ((forks[i]==-1) || ((forks[i]==-1) && ((philosopher[forks[i]]==0) || (philNum==forks[i])))) {
        // if fork is unused or philosopher last using the fork has returned to thinking 
        // or if the philosopher last using the fork was the current one
        philosopher[philNum]=1; //acquiring a fork makes the philosopher hungry
        forks[i]=philNum; //fork is labelled to be in use by current philosopher
        return 0;
    } else {
        return -1;
    }
}

void eat(int i) {
    philosopher[i] = 2;
}

void think(int i) {
    philosopher[i] = 0;
}


int putFork() {
    // doesn't do anything. 
    // the lesser the interaction with forks[], the lesser the chances of conflict
    // returning to thinking will allow other philosophers to take the fork
    // fork doesn't inherently need to store the state of use, only the user
}


void *philosphise(void *_i) {
    int i = *((int *) _i);

    while (active) {
        if (i<4) {
            printf("i :: [%d %d %d %d %d]\n", philosopher[0], philosopher[1], philosopher[2], philosopher[3], philosopher[4]);
            if (pickFork(i, i)==-1) {continue;}
            if (pickFork((i+1)%5, i)==-1) {continue;}
            eat(i);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            sleep(1);
            think(i);
            printf("Philosopher %d think_ [%d::%d]!\n", i, i, (i+1)%5);
            
        } else {
            printf("i :: [%d %d %d %d %d]\n", philosopher[0], philosopher[1], philosopher[2], philosopher[3], philosopher[4]);
            if (pickFork((i+1)%5, i)==-1) {continue;}
            if (pickFork(i, i)==-1) {continue;}
            eat(i);
            printf("Philosopher %d eating [%d::%d]!\n", i, i, (i+1)%5);
            sleep(1);
            think(i);
            printf("Philosopher %d think_ [%d::%d]!\n", i, i, (i+1)%5);
        }
    }
}

int main() {
    pthread_t pids[5];
    
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
