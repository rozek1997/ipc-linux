#include "so_project.h"



void sigHandlerChildren(int signal) {             //funkcja obslugujaca lock SIGINT z procesu 1

    if (signal == SIGINT) {
        kill(getppid(), SIGUSR1);
    }
    if (signal == SIGTSTP) {
        kill(getppid(), SIGUSR2);
    }
    if (signal == SIGCONT) {
        kill(getppid(), SIGHUP);
    }

    if (signal == SIGUSR1) {//konczenie pracy
        kill(getppid(), SIGTTIN);
        shmDetach();
        kill(getpid(), SIGKILL);
    }
    if (signal == SIGUSR2) {//zatrzymywanie procesu
        lock = 1;
        char message[5];
        iterator--;
        read(fifoDescriptor, message, sizeof(message));

    }
    if (signal == SIGHUP) {//wznawianie procesu
        lock = 0;
    }

}

void sigHandlerParent(int signal) {

    if (signal == SIGUSR1) {
        kill(process1PID, SIGUSR1);
        kill(process2PID, SIGUSR1);
        kill(process3PID, SIGUSR1);
    }
    if (signal == SIGUSR2) {
        kill(process1PID, SIGUSR2);
        kill(process2PID, SIGUSR2);
        kill(process3PID, SIGUSR2);
    }
    if (signal == SIGHUP) {
        kill(process1PID, SIGHUP);
        kill(process2PID, SIGHUP);
        kill(process3PID, SIGHUP);
    }
    if (signal == SIGTTIN) {//konczenie procesu macierzytego
        close(fifoDescriptor);
        unlink(myFIFO);
        shmRemove();
        semctl(semId, 0, IPC_RMID);
        kill(getpid(), SIGKILL);
    }


}

void signalConfig(sigset_t *mask_set, struct sigaction *act) {

    umask(0);

    sigfillset(mask_set);              //ustawainie wszystkich bitow zmiennej mask_set na 1
    sigdelset(mask_set, SIGUSR1);
    sigdelset(mask_set, SIGUSR2);
    sigdelset(mask_set, SIGHUP);
    sigdelset(mask_set, SIGCONT);
    sigdelset(mask_set, SIGINT);
    sigdelset(mask_set, SIGTSTP);
    sigdelset(mask_set, SIGTTIN);

    sigprocmask(SIG_SETMASK, mask_set, NULL);
    act->sa_handler = &sigHandlerChildren;
    act->sa_mask = *mask_set;
    act->sa_flags = 0;
    sigaction(SIGTSTP, act, NULL);
    sigaction(SIGINT, act, NULL);
    sigaction(SIGCONT, act, NULL);
    sigaction(SIGUSR1, act, NULL);
    sigaction(SIGUSR2, act, NULL);
    sigaction(SIGHUP, act, NULL);
}

void signalReconfig(sigset_t *mask_set, struct sigaction *act) {
    sigaddset(mask_set, SIGCONT);
    sigaddset(mask_set, SIGINT);
    sigaddset(mask_set, SIGTSTP);

    sigprocmask(SIG_BLOCK, mask_set, NULL);
    act->sa_handler = &sigHandlerParent;
    act->sa_mask = *mask_set;
    act->sa_flags = 0;
    sigaction(SIGUSR1, act, NULL);
    sigaction(SIGUSR2, act, NULL);
    sigaction(SIGHUP, act, NULL);
    sigaction(SIGTTIN, act, NULL);
}