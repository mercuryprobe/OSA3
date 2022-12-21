#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>



char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char strings[50][5];


void constructor() {
    for (int i =0; i<50; i++) {
        for (int j = 0; j<4; j++) {
            strings[i][j] = characters[rand()%52];
        }
    }
}

int main() {
    // puts("P1");
    // reference: The Linux Programming Interface, Michael Kerrisk
    const char *lockLoc = "/semLock";
    const char *semLocation = "/semSync";
    const char *location = "/sharedmem";
    struct timespec start;
    struct timespec stop;
    double billion = 1000000000;
    sem_t *sem;
    sem = sem_open(semLocation, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
    sem_t *lock;
    lock = sem_open(lockLoc, O_RDWR, S_IRUSR | S_IWUSR, 1);

    constructor();
    
    // https://man7.org/linux/man-pages/man3/shm_open.3.html, example
    int filedescriptor = shm_open(location, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (filedescriptor<0) {
        perror("SHM File Creation failed");
    }

    if (ftruncate(filedescriptor, 2048) == -1) {
        perror("[SERVER] Ftruncate failed!");
    }
    void *pointer = mmap(0, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, filedescriptor, 0);
    if (pointer == MAP_FAILED) {
        perror("[SERVER] Mapping shm object failed!");
        return 0;
    }
    
    int i  = 0;
    clock_gettime(CLOCK_REALTIME, &start);    
    while (i<50) {
        sem_wait(sem);
        char curString[64];
        const char space[2] = " ";
        int j = i+5;
        for (i; i<(j); i++) {
            // reference: https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
            int length = snprintf(NULL, 0, "%d", i);
            char* numStr = malloc(length + 1);

            snprintf(numStr, length+1, "%d", i);
            strcat(curString, numStr);       //add integer
            strcat(curString, space);        //add space
            strcat(curString, strings[i]);   //add current string
            strcat(curString, space);        //add space
        }
        // puts(curString);
        
        memcpy(pointer, curString, sizeof(curString));
        pointer += (sizeof(curString)+1);
        
        sem_post(sem);
        // printf("[SERVER] Characters written: %d\n", charWritten);
        usleep(200);
        puts("ok0");
        sem_wait(lock);
        puts("ok1");
        sem_wait(sem);
        puts("ok2");
        sem_post(lock);
        puts("ok3");
        char received[64];
        memcpy(received, pointer, sizeof(received));
        // printf("[SERVER] Sent: %s\n", curString);
        printf("[SERVER] Received index: %s\n\n", received);
        pointer += (sizeof(received)+1);
        sem_post(sem);
        for (int k =0; k<64; k++) {
            curString[k] = 0;
        }    
    }
    munmap(pointer, 2048);
    close(filedescriptor);
    shm_unlink(location);
    
    clock_gettime(CLOCK_REALTIME, &stop);
    double duration = stop.tv_sec + stop.tv_nsec/billion - (start.tv_sec + start.tv_nsec/billion);
    
    printf("SHM runtime: %lf\n", duration);
    return 0;
}