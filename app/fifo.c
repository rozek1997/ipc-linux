//
// Created by rozek1997 on 05.01.2020.
//

#include <err.h>
#include "so_project.h"

void openFifo(int openingFlag){
    fifoDescriptor=mkfifo(myFIFO, 0666);

    if(fifoDescriptor == -1 && errno != EEXIST){         //warto zwrocic uwage, bo gdy plik juz istnieje fifoDescriptor rowniez przyjmie wartosc -1
        warn("Fifo already exist");
//        kill(getpid(), SIGINT);
//        exit(EXIT_FAILURE);
    }
    fifoDescriptor = open(myFIFO, openingFlag);        //otwwiranie fifo
    if(fifoDescriptor == -1){
        perror("Error opening fifo");
        kill(getpid(), SIGINT);
        exit(EXIT_FAILURE);
    }

}
