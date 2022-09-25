#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#define CALLER0_FIFO "/tmp/caller0"
#define CALLER1_FIFO "/tmp/caller1"
#define BUFF_LEN 150

void make_fifo(){
    mkfifo(CALLER0_FIFO, 0666);
    mkfifo(CALLER1_FIFO, 0666);
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("%s: Invalid Arguments\nUsage: %s <caller_no>\n", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    char self_fifo[15], peer_fifo[15];
    char buffer[BUFF_LEN];
    if (argv[1][0] == '0'){
        strncpy(self_fifo, CALLER0_FIFO, 15);
        strncpy(peer_fifo, CALLER1_FIFO, 15);
    }
    else if (argv[1][0] == '1'){
        strncpy(self_fifo, CALLER1_FIFO, 15);
        strncpy(peer_fifo, CALLER0_FIFO, 15);
    }
    else{
        printf("Please choose among (0/1)!");
        exit(EXIT_FAILURE);
    }
    make_fifo();
    pid_t sender = fork();
    if (sender < 0){
        printf("Unexpected error while forking process!");
        exit(EXIT_FAILURE);
    }else if(sender){
        while(true){
            int fd = open(self_fifo, O_RDONLY);
            read(fd, buffer, BUFF_LEN);
            close(fd);
            printf("%s\n", buffer);
            if(strcmp(buffer, "EOF") == 0) break;
        }
    }else{
        while(true){
            scanf("%[^\n]%*c", buffer);
            int fd = open(peer_fifo, O_WRONLY);
            write(fd, buffer, strlen(buffer)+1);
            close(fd);
        }
    }
    return 0;
}
