#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int main(int argc, char** argv){
    pid_t child_x, child_y;
    pid_t parent = getpid();
    child_x = fork();
    if (child_x < 0){
        printf("Error while forking!");
        exit(EXIT_FAILURE);
    }else if(child_x == 0){
        for (int i = 0; i < 10; i++){
            printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", parent, getpid(), i+1);
            sleep(2);
        }
        
    }else{
        child_y = fork();
        if (child_y < 0){
            printf("Error while forking!");
            exit(EXIT_FAILURE);
        }else if(child_y == 0){
            for (int i = 0; i < 10; i++){
                printf("Parent Process ID : %d, Child Process ID : %d, Iteration no.: %d\n", parent, getpid(), i+1);
                sleep(1);
            }        
        }
    }
    wait(NULL);
    wait(NULL);
    return 0;    

}