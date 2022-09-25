#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#define MUTEX "mutex"

void display(unsigned int t, sem_t* mutex){
    for (int i = 0; i < 10; i++){
        
        printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", getppid(), getpid(), i+1);
        sleep(t);
        sem_post(mutex);
    }    
}

int main(int argc, char** argv){
    sem_t *mutex;
    pid_t pidx = -1, pidy = -1;
    unsigned int shared = 0;
    sem_unlink(MUTEX);
    if((mutex = sem_open(MUTEX, O_CREAT, 0666, 0)) == SEM_FAILED){
        printf("Error while opening semaphore!");
        exit(EXIT_FAILURE);
    }
    if ((pidx = fork()) < 0){
        printf("Error while forking!");
        exit(EXIT_FAILURE);
    }else if (pidx == 0){
        display(1, mutex);
    }else{
        if ((pidy = fork()) < 0){
            printf("Error while forking!");
            exit(EXIT_FAILURE);
        }else if (pidy == 0){
            sem_wait(mutex);
            display(1, mutex);
        }else{
            wait(NULL);
            wait(NULL);
            if (sem_unlink(MUTEX) == -1){
                perror("Semaphore unlink failed!");
                return 1;
            }            
        }
    }
    return 0;    
}