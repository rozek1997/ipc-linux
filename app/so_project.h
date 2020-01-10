
#ifndef SO_PROJEKT_SO_PROJECT_H
#define SO_PROJEKT_SO_PROJECT_H

#define MILISEC_IN_SEC 1000000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>

#include <sys/msg.h>

int lock;
char* myFIFO;
int fifoDescriptor;
int iterator;
int shmId;
int semId;
char * shm;
const int MAX_MESSAGE_SIZE;
FILE* pidsFileDescriptor;
char* pidsFileName;

pid_t process1PID, process2PID, process3PID;

void process1Init(char ** argv);
//void process2Init();
void process3Init();

void signalConfig(sigset_t *mask_set, struct sigaction *act);
void signalReconfig(sigset_t *mask_set, struct sigaction *act);

void semUnlock(int semNum);
void semLock(int semNum);
void semInit();

void shmCreate();
void shmAttach();
void shmDetach();
void shmRemove();


void openFifo(int openingFlag);
//
void shmSendMessage(char *message, FILE *fp);
void printProcessPID();
FILE *openFile(char *filename);

#endif //SO_PROJEKT_SO_PROJECT_H
