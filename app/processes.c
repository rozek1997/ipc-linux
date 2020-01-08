//
//#include <semaphore.h>
#include "so_project.h"



__always_inline void shmSendMessage(char *message, FILE *fp) {

    memset(message, 0, MAX_MESSAGE_SIZE);
    while (lock) {}
    semLock(0);
    fgets(message, MAX_MESSAGE_SIZE, fp);
    strncpy(shm, message, MAX_MESSAGE_SIZE);
    semUnlock(1);
    usleep(0.1 * MILISEC_IN_SEC);

}


void printProcessPID() {
    fprintf(stderr, "Process ID: %d\n", getpid());
}

FILE *openFile(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Nie udalo sie otworzyc pliku %s\n", filename);
        kill(getpid(), SIGINT);
        exit(EXIT_FAILURE);
    }

    return fp;
}


void process1Init(char **argv) {

    char message[MAX_MESSAGE_SIZE];

    shmAttach();
    printProcessPID();
    usleep(0.5 * MILISEC_IN_SEC);
    if (strncmp(argv[1], "-i", 2) == 0) {
        while (1) {
//            if(getc(stdin)==EOF)
            printf("\n%s", "Podaj wiadomość do wysłania: ");
            shmSendMessage(message, stdin);
        }
    } else if (strncmp(argv[1], "-u", 2) == 0) {

        FILE *fp = openFile("/dev/urandom");
        while (1) {
            shmSendMessage(message, fp);
        }

    } else if (strncmp(argv[1], "-f", 2) == 0) {

        FILE *fp = openFile(argv[2]);
        while (!feof(fp)) {
            shmSendMessage(message, fp);
        }
        kill(getpid(), SIGINT);
    }
}

//void process2Init() {
//
//    const int CONVERTED_MESSAGE_SIZE = 2 * MAX_MESSAGE_SIZE;
//    char message[CONVERTED_MESSAGE_SIZE];
//    char buffer[MAX_MESSAGE_SIZE];
//
//    printProcessPID();
//
//    iterator = 0;
//    int convertingIterator = 0;
//
//
//    while (1) {
//        memset(message, 0, CONVERTED_MESSAGE_SIZE);
//        memset(buffer, 0, MAX_MESSAGE_SIZE);
//        while (lock) {}
//        semLock(1);
//        strncpy(buffer, shm, MAX_MESSAGE_SIZE);
//        for (iterator = 0, convertingIterator = 0; iterator < strlen(buffer); iterator++, convertingIterator += 2) {
//            sprintf((char *) (message + convertingIterator), "%02x", buffer[iterator]);
//        }
//
//        write(fifoDescriptor, message, strlen(message) + 1);
//        semUnlock(0);
//    }
//
//}

void process3Init() {

    const int CONVERTED_MESSAGE_SIZE = 2 * MAX_MESSAGE_SIZE;
    char message[CONVERTED_MESSAGE_SIZE];
    short printPretty = 0;

    openFifo(O_RDONLY);
    printProcessPID();

    while (1) {
        memset(message, 0 , CONVERTED_MESSAGE_SIZE);
        while (lock) {}
        read(fifoDescriptor, message, CONVERTED_MESSAGE_SIZE);
        int messageSize = strlen(message);
        for (iterator = 0; iterator < messageSize; iterator += 2) {

            if (printPretty == 15) {
                fprintf(stderr, "\n");
                printPretty = 0;
            }
            fprintf(stderr, "%c%c ", message[iterator], message[iterator + 1]);
            printPretty++;
        }

    }
}

