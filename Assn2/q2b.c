#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#define S1 "sem1"
#define S2 "sem2"

void display(unsigned int t, sem_t* sw, sem_t* sp){
    for (int i = 0; i < 10; i++){
        sem_wait(sw);
        printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", getppid(), getpid(), i+1);
        sem_post(sp);
        sleep(t);
    }    
}

int main(int argc, char** argv){
    sem_t *p_sem1, *p_sem2;
    pid_t pidx = -1, pidy = -1;
    unsigned int shared = 0;
    if((p_sem1 = sem_open(S1, O_CREAT, 0666, 1)) == SEM_FAILED || (p_sem2 = sem_open(S2, O_CREAT, 0666, 0)) == SEM_FAILED){
        printf("Error while opening semaphore!");
        exit(EXIT_FAILURE);
    }
    if ((pidx = fork()) < 0){
        printf("Error while forking!");
        exit(EXIT_FAILURE);
    }else if (pidx == 0){
        display(1, p_sem1, p_sem2);
    }else{
        if ((pidy = fork()) < 0){
            printf("Error while forking!");
            exit(EXIT_FAILURE);
        }else if (pidy == 0){
            display(1, p_sem2, p_sem1);
        }else{
            wait(NULL);
            wait(NULL);
            if (sem_unlink(S1) == -1 || sem_unlink(S2) == -1){
                perror("Semaphore unlink failed!");
                return 1;
            }            
        }
    }
    return 0;    
}