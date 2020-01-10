#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "so_project.h"

int lock = 0;
char *myFIFO = "/tmp/my_fifo";
int fifoDescriptor = -1;
int iterator = 0;
int shmId = 0;
int semId = 0;
const int MAX_MESSAGE_SIZE = 15 + 1;
FILE* pidsFileDescriptor = NULL;
char * pidsFileName = "/tmp/myPIDs";


void inputParamsCheck(int argc, char **argv) {
    if (argc >= 2) {
        if (argv[1][0] == '-') {
            if (strncmp(argv[1], "-i", 2) == 0)
                printf("Program bedzie pracowal w trybie interaktywnym\n");
            else if (strncmp(argv[1], "-u", 2) == 0)
                printf("Program bedzie pobieral znaki z /dev/urandom\n");
            else if (strncmp(argv[1], "-f", 2) == 0) {
                if (argc > 2) {
                    if (access(argv[2], R_OK) == -1) {
                        fprintf(stderr, "Nie można otworzyć pliku");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    fprintf(stderr, "Nie podano scieżki do pliku");
                    exit(EXIT_FAILURE);
                }

                printf("Program bedzie pracowal w trybie pobierania z pliku %s\n", argv[2]);

            } else {
                fprintf(stderr, "Wybrano niewłasciwy tryb pracy programu\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Nie poprawne parametry pracy programu\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr,
                "Nie poprawne parametry pracy programu\n");
        exit(EXIT_FAILURE);
    }
}

void init() {
    semInit();
    shmCreate();
}

void savePIDToFile(){

    pidsFileDescriptor = fopen(pidsFileName, "wt");
    if(pidsFileDescriptor == NULL){
        fprintf(stderr, "ID procesów nie zostały zapisane do pliku");
    }

    fprintf(pidsFileDescriptor, "Process%d\n", process1PID);
    fprintf(pidsFileDescriptor, "Process%d\n", process2PID);
    fprintf(pidsFileDescriptor, "Process%d", process3PID);

    fclose(pidsFileDescriptor);
}

int main(int argc, char **argv) {

    inputParamsCheck(argc, argv);
    sigset_t mask_set;
    struct sigaction act;

    init();
    signalConfig(&mask_set, &act);

    if ((process1PID = fork()) == 0) {
        process1Init(argv);
        return 0;
    }
    if ((process2PID = fork()) == 0) {

        const int CONVERTED_MESSAGE_SIZE = 2 * MAX_MESSAGE_SIZE;
        char message[CONVERTED_MESSAGE_SIZE];
        char buffer[MAX_MESSAGE_SIZE];

        shmAttach();
        openFifo(O_WRONLY);
        printProcessPID();

        iterator = 0;
        int convertingIterator = 0;

        semUnlock(0);
        while (1) {


            memset(message, 0, CONVERTED_MESSAGE_SIZE);
            memset(buffer, 0, MAX_MESSAGE_SIZE);
            while (lock) {}
            semLock(1);
            strncpy(buffer, shm, MAX_MESSAGE_SIZE);
            for (iterator = 0, convertingIterator = 0; iterator < strlen(buffer); iterator++, convertingIterator += 2) {
                sprintf((char *) (message + convertingIterator), "%02x", buffer[iterator]);
            }

            write(fifoDescriptor, message, strlen(message) + 1);
            semUnlock(0);

        }

        return 0;
    }

    if ((process3PID = fork()) == 0) {
        process3Init();
        return 0;
    }

    signalReconfig(&mask_set, &act);
    savePIDToFile();

    while (1) {
        pause();
    }

    return 0;
}



