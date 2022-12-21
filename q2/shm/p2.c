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

int main() {
    
    // puts("P2");
    // reference: The Linux Programming Interface, Michael Kerrisk
    // const char *lockLoc = "/semLock";
    const char *semLocation = "/semSync";
    const char *location = "/sharedmem";
    const char space[2] = " ";
    sem_t *sem;
    sem = sem_open(semLocation, O_RDWR, S_IRUSR | S_IWUSR, 1);
    // sem_t *lock;
    // if ((lock = sem_open(lockLoc, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1))==SEM_FAILED) {
    //     perror("[CLIENT] Lock error");
    // }

    // sem_wait(lock);
    
    usleep(5);
    int filedescriptor = shm_open(location, O_RDWR, S_IRUSR | S_IWUSR);

    if (ftruncate(filedescriptor, 2048) == -1) {
        perror("[CLIENT] Ftruncate failed!");
    }
    void *pointer = mmap(0, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, filedescriptor, 0);
    if (pointer == MAP_FAILED) {
        perror("[CLIENT] Mapping shm object failed!");
        return 0;
    }

    for (int l =0; l<10; l++) {
        sem_wait(sem);
        // sem_post(lock);

        char received[64];
        memcpy(received, pointer, sizeof(received));
        printf("[CLIENT] Received: %s\n", received);
        // received[63] = 0;
        pointer += (sizeof(received)+1);

        char* tokenRecv;
        tokenRecv = strtok(received, space);

        char splitString[16][64];
        int i = 0;
        while (tokenRecv!=NULL) {
            strcpy(splitString[i], tokenRecv);
            // printf("%s ", splitString[i]);
            tokenRecv = strtok(NULL, space);
            i+=1;
        }
        // puts("");
        // printf("Splitstring[8]: %s", splitString[8]);
        memcpy(pointer, splitString[8], sizeof(splitString[8]));
        pointer += (sizeof(splitString[8])+1);
        sem_post(sem);
        while((strcmp(currentVal, "]")==0)) {
            memcpy(currentVal, pointer, sizeof("]"));
            // usleep(1);
        };
        // sem_wait(lock);
    }
    munmap(pointer, 2048);
    close(filedescriptor);

    return 0;
}