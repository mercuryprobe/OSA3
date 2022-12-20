#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>

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
    constructor();
    // reference: The Linux Programming Interface, Michael Kerrisk
    int sfiledescriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfiledescriptor<0) {
        perror("[SERVER] Socket initiation failed:");
        return 0;
    } else {
        puts("[SERVER] Initiation successful!");
    }

    struct sockaddr_un sockAddress;
    memset(&sockAddress, 0, sizeof(struct sockaddr_un));
    sockAddress.sun_family = AF_UNIX;
    strncpy(sockAddress.sun_path, "/tmp/socketFile", sizeof(sockAddress.sun_path) - 1);
    
    if ((bind(sfiledescriptor, (struct sockaddr *) &sockAddress, sizeof(sockAddress))!=0)) {
        perror("[SERVER] Socket bind failed:");
        return 0;
    } else {
        puts("[SERVER] Bind successful!");
    }

    if ((listen(sfiledescriptor, 64))!=0) {
        perror("[SERVER] Socket listen failed:");
        return 0;
    } else {
        puts("[SERVER] listen successful!");
    }

    int cfiledescriptor = accept(sfiledescriptor, NULL, NULL);
    if (cfiledescriptor<0) {
        puts("[SERVER] Client accept failed!");
        return 0;
    } else {
        puts("[SERVER] Client accept successful!");
    }
    int i  = 0;    
    while (i<50) {
        char curString[56];
        const char space[2] = " ";
        int j = i+5;
        for (i; i<(j+5); i++) {
            // reference: https://stackoverflow.com/questions/8257714/how-to-convert-an-int-to-string-in-c
            int length = snprintf(NULL, 0, "%d", i);
            char* numStr = malloc(length + 1);
            puts("1");
            snprintf(numStr, length+1, "%d", i);
            puts("2");
            strcat(curString, space);        //add space
            strcat(curString, numStr);       //add integer
            strcat(curString, strings[i]);   //add current string
            strcat(curString, space);        //add space
            puts("3");
            puts(curString);
        }
        puts(curString);
        if ((write(cfiledescriptor, &curString, sizeof(curString)))!=0) {
            perror("[SERVER] Socket write failed");
            return 0;
        }
        
        char received[4];
        if (read(cfiledescriptor, received, sizeof(received))<0) {
            perror("[SERVER] Socket read failed");
        };
        printf("Sent:\n%s\n", curString);
        printf("Received index: %s\n", received);
    }
    
    close(sfiledescriptor);
    close(cfiledescriptor);
    return 0;
}