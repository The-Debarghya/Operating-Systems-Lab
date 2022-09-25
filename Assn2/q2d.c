#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t* mutexX;
sem_t* mutexY;
sem_t* mutexZ;
int main(int argc, char** argv){
    sem_unlink("mutexX");
    sem_unlink("mutexY");
    sem_unlink("mutexZ");

    mutexX = sem_open("mutexX", O_CREAT, 0644, 0);
    mutexY = sem_open("mutexY", O_CREAT, 0644, 0);
    mutexZ = sem_open("mutexZ", O_CREAT, 0644, 0);
    pid_t child_x, child_y, child_z;
    pid_t parent = getpid();
    child_x = fork();
    if (child_x < 0){
        printf("Error while forking!");
        exit(EXIT_FAILURE);
    }else if(child_x == 0){
        for (int i = 0; i < 10; i++){
            int num = rand() % 5;
            if(i != 0){
                sem_wait(mutexX);
            }
            printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", parent, getpid(), i+1);
            sleep(num);
            sem_post(mutexY);
        }
        
    }else{
        child_y = fork();
        if (child_y < 0){
            printf("Error while forking!");
            exit(EXIT_FAILURE);
        }else if(child_y == 0){
            for (int i = 0; i < 10; i++){
                int num = rand() % 5;
                sem_wait(mutexY);
                printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", parent, getpid(), i+1);
                sleep(num);
                sem_post(mutexZ);
            }        
        }else{
            child_z = fork();
            if (child_z < 0){
                printf("Error while forking!");
                exit(EXIT_FAILURE);
            }else if(child_z == 0){
                for (int i = 0; i < 10; i++){
                    int num = rand() % 5;
                    sem_wait(mutexZ);
                    printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", parent, getpid(), i+1);
                    sleep(num);
                    sem_post(mutexX);
                }        
            }
        }
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    sem_destroy(mutexX);
    sem_destroy(mutexY);
    sem_destroy(mutexY);
    return 0;   
}