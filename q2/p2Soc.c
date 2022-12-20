#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h>

int main() {
    // reference: The Linux Programming Interface, Michael Kerrisk
    int filedescriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    if (filedescriptor<0) {
        perror("[CLIENT] Socket initiation failed:");
        return 0;
    } else {
        puts("[CLIENT] Initiation successful!");
    }

    struct sockaddr_un sockAddress;
    memset(&sockAddress, 0, sizeof(struct sockaddr_un));
    sockAddress.sun_family = AF_UNIX;
    strncpy(sockAddress.sun_path, "/tmp/socketFile", sizeof(sockAddress.sun_path) - 1);
    sleep(1);
    if ((connect(filedescriptor, (struct sockaddr *) &sockAddress, sizeof(sockAddress))!=0)) {
        perror("[CLIENT] Socket connect failed:");
        return 0;
    } else {
        puts("[CLIENT] Connection successful!");
    }

    sleep(1);
    const char space[2] = " ";
    char received[56];
    int recSize;
    while (recSize = read(filedescriptor, received, 56)>0) {
        if (recSize<0) {
            perror("[CLIENT] Socket read failed");
            return 0;
        } else {
            printf("[CLIENT] Read successful! %s\n", recSize);
        }
        received[recSize] = 0;
        printf("[CLIENT] Received: %s\n", received);

        char* tokenRecv;
        tokenRecv = strtok(received, space);

        char splitString[16][16];
        int i = 0;
        while (tokenRecv!=NULL) {
            strcpy(splitString[i], tokenRecv);
            tokenRecv = strtok(NULL, space);
            i+=1;
        }


        if ((write(filedescriptor, splitString[8], sizeof(splitString[8])))<0) {
            perror("[CLIENT] Socket write failed");
            return 0;
        }
    };
    
    close(filedescriptor);
    return 0;
}