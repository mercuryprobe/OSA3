#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <time.h>
#include <sys/stat.h>

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
    // reference: The Linux Programming Interface, Michael Kerrisk
    const char *location = "/tmp/fifoFile";
    struct timespec start;
    struct timespec stop;
    double billion = 1000000000;
    
    constructor();
    
    
    int i  = 0;
    if (mkfifo(location, S_IRUSR | S_IWUSR | S_IWGRP)<0) {
        perror("FIFO File Creation failed");
    }
    clock_gettime(CLOCK_REALTIME, &start);    
    while (i<50) {
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
        
        int out = open(location, O_WRONLY);
        if ((write(out, curString, sizeof(curString)))<0) {
            perror("[SERVER] FIFO write failed");
            return 0;
        }
        close(out);
        // printf("[SERVER] Characters written: %d\n", charWritten);
        
        int in = open(location, O_RDONLY);
        char received[64];
        if (read(in, received, 64)<0) {
            perror("[SERVER] FIFO read failed");
        };
        // printf("[SERVER] Sent: %s\n", curString);
        printf("[SERVER] Received index: %s\n\n", received);
        for (int k =0; k<56; k++) {
            curString[k] = 0;
        }
        close(in);
    }
    
    clock_gettime(CLOCK_REALTIME, &stop);
    double duration = stop.tv_sec + stop.tv_nsec/billion - (start.tv_sec + start.tv_nsec/billion);

    printf("FIFO runtime: %lf\n", duration);
    return 0;
}