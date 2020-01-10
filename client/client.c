#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_PROC_NUMBER 3


pid_t procPIDs[MAX_PROC_NUMBER];


FILE *openFile() {
    FILE *fp = fopen("/tmp/myPIDs", "r");
    if (fp == NULL) {
        fprintf(stderr, "Nie udalo sie otworzyc pliku %s\n", "/tmp/myPIDs");
        exit(EXIT_FAILURE);
    }

    return fp;
}

void parseInput(FILE *fp) {

    size_t len = 20;
    char buffer[len];
    char procPIDString[len];
    char stringToFind[] = "Process";
    char *bufferPointer = buffer;
    short iterator = 0;

    int procPIDInt = 0;


    while (getline(&bufferPointer, &len, fp) != -1) {

        if (strstr(buffer, stringToFind) != NULL) {
            memcpy(procPIDString, &buffer[7], 5);
            procPIDString[5] = '\0';
            procPIDInt = atoi(procPIDString);
            procPIDs[iterator] = procPIDInt;
            iterator++;
            if (iterator == 3) break;
        }
    }
}

int signalInput(int *signalChoice) {

    printf("Wybierz sygnal do wyslania: \n");
    printf("%d - SIGINT %s\t", 0, "Zakoncz");
    printf("%d - SIGTSTP %s\t", 1, "Wstrzymaj");
    printf("%d - SIGCONT %s\t", 2, "Wznów");
    printf("Wybor: ");
    scanf("%d", signalChoice);

    if ((*signalChoice != 0&&*signalChoice != 1)&&*signalChoice != 2) {
        fprintf(stderr, "Błedny wybór \n");
        return -1;
    }

    return 0;
}

int pidInput(int *pidChoice) {

    printf("Wybierz numer procesu: \n");
    for (short i = 0; i < MAX_PROC_NUMBER; i++)
        printf("%d - PID: %d \t", i, procPIDs[i]);
    printf("Wybor: ");
    scanf("%d", pidChoice);

    if ((*pidChoice != 0&&*pidChoice != 1)&&*pidChoice != 2) {
        fprintf(stderr, "Błedny wybór \n");
        return -1;
    }

    return 0;
}



void input() {

    int pidChoice = -1;
    int signalChoice = -1;
    while (1) {
        if (pidInput(&pidChoice) < 0) continue;
        if (signalInput(&signalChoice) < 0) continue;

        if (signalChoice == 0) {
            kill(procPIDs[pidChoice], SIGINT);
            break;
        }
        if (signalChoice == 1) kill(procPIDs[pidChoice], SIGTSTP);
        if (signalChoice == 2) kill(procPIDs[pidChoice], SIGCONT);

        printf("Sygnał został wysłany do procesu: %d\n", procPIDs[pidChoice]);
    }
}


int main() {

    FILE *fp = openFile();
    parseInput(fp);
    input();

    fclose(fp);
    return 0;

}