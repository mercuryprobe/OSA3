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

#define semLocation "/tmp/semSync"

int main() {
    usleep(10);
    // reference: The Linux Programming Interface, Michael Kerrisk
    const char *location = "/sharedmem";
    const char space[2] = " ";
    sem_t *sem = sem_open(semLocation, O_RDWR, S_IRUSR | S_IWUSR, 1);
    
    int filedescriptor = shm_open(location, O_RDWR, S_IRUSR | S_IWUSR);

    if (ftruncate(filedescriptor, 2048) == -1) {
        perror("[CLIENT] Ftruncate failed!");
    }
    char *pointer = mmap(0, 2048, PROT_READ | PROT_WRITE, MAP_SHARED, filedescriptor, 0);
    if (pointer == MAP_FAILED) {
        perror("[CLIENT] Mapping shm object failed!");
        return 0;
    }

    
    for (int l =0; l<10; l++) {
        sem_wait(sem);
        char received[64];
        sscanf(pointer, "%s", received);
        printf("[CLIENT] Received: %s\n", received);
        received[63] = 0;

        char* tokenRecv;
        tokenRecv = strtok(received, space);

        char splitString[16][16];
        int i = 0;
        while (tokenRecv!=NULL) {
            strcpy(splitString[i], tokenRecv);
            // printf("%s ", splitString[i]);
            tokenRecv = strtok(NULL, space);
            i+=1;
        }
        // puts("");
        // printf("Splitstring[8]: %s", splitString[8]);
        sprintf(pointer, "%s", splitString[8]);
        sem_post(sem);
    }
    munmap(pointer, 2048);
    close(filedescriptor);

    return 0;
}