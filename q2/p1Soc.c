#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>


char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
char strings[50][5];


void constructor() {
    for (int i =0; i<50; i++) {
        for (int j = 0; j<4; j++) {
            strings[i][j] = characters[rand()%52];
        }
        puts(strings[i]);
    }
}

int main() {
    // reference: The Linux Programming Interface, Michael Kerrisk
    int sfiledescriptor = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfiledescriptor!=0) {
        perror("[SERVER] Socket initiation failed:");
        return 0;
    }

    struct sockaddr_un sockAddress;
    memset(&sockAddress, 0, sizeof(struct sockaddr_un));
    sockAddress.sun_family = AF_UNIX;
    strncpy(sockAddress.sun_path, "socket/socketFile", sizeof(sockAddress.sun_path) - 1);
    
    if ((bind(sfiledescriptor, (struct sockaddr *) &sockAddress, sizeof(sockAddress))!=0)) {
        perror("[SERVER] Socket bind failed:");
        return 0;
    }
    if ((listen(sfd, 64))!=0) {
        perror("[SERVER] Socket bind failed:");
        return 0;
    }

    int cfiledescriptor = accept(sfiledescriptor, NULL, NULL);
    
    if ((write(cfiledescriptor, &strings[], sizeof(strings[0])))!=0) {
        perror("[SERVER] Socket write failed")
    }



    
    return 0;
}