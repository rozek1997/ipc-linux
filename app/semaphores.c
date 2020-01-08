//
// Created by rozek1997 on 08.01.2020.
//

#include <err.h>
#include "so_project.h"

void semInit(){

    semId = semget(4008, 2, 0666|IPC_CREAT);
    if (semId == -1)
        errx(2, "Blad tworzenia semafora!");

    int errorValue = semctl(semId, 0, SETVAL, 1);
    if (errorValue == -1)
        errx(3, "Blad ustawiania semafora pierwszego!");

    errorValue = semctl(semId, 1, SETVAL, 0);
    if ( errorValue == -1)
        errx(3, "Blad ustawiania semafora drugiego!");

}

__always_inline void semLock(int semNum) {

    struct sembuf opr;
    opr.sem_num = semNum;
    opr.sem_op = -1;
    opr.sem_flg = 0;// blokowanie
    while (semop(semId, &opr, 1) == -1);
}

/* odblokowanie semafora */
__always_inline void semUnlock(int semNum) {


    struct sembuf opr;

    opr.sem_num = semNum;
    opr.sem_op = 1;
    opr.sem_flg = 0;
    while (semop(semId, &opr, 1) == -1);

}

