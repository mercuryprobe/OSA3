#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    // reference: The Linux Programming Interface, Michael Kerrisk
    const char *location = "/tmp/fifoFile";
    sleep(1);
    const char space[2] = " ";
    
    
    for (int l =0; l<10; l++) {
        int in = open(location, O_RDONLY);

        char received[64];
        if (read(in, received, 64)<0) {
            perror("[CLIENT] FIFO read failed");
            return 0;
        } else {
            printf("[CLIENT] Read successful!\n");
        }
        printf("[CLIENT] Received: %s\n", received);
        received[63] = 0;

        close(in);
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
        int out = open(location, O_WRONLY);
        if ((write(out, splitString[8], sizeof(splitString[8])))<0) {
            perror("[CLIENT] FIFO write failed");
            return 0;
        }
        close(out);
    };

    return 0;
}