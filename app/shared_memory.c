
#include <err.h>
#include "so_project.h"



void shmCreate() {
    key_t shmkey;
    /* Tworzymy klucz dla pamieci dzielonej. */
    if ((shmkey = ftok(".", 'L')) == -1)
        errx(1, "Blad tworzenia klucza!");

    /* Tworzymy segment pamieci dzielonej i zapsiujemy do zmiennej shmId która istniej w mainie*/
    shmId = shmget(shmkey, sizeof(char) * 16, IPC_CREAT|0600);
    if (shmId < 0)
        errx(2, "Blad tworzenia segmentu pamieci dzielonej!");

}

void shmAttach(){
    shm = shmat(shmId, NULL, 0);
    if ((void *) shm == (void *) -1)//do kazdego przypisuje ta sama pamiec dzielona dlatego wywala
        errx(3, "Blad przylaczania pamieci dzielonej!");

}

void shmDetach() {
    shmdt(shm);
}

void shmRemove() {
    shmDetach();
    shmctl(shmId, IPC_RMID, NULL);
}
