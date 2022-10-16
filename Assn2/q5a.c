#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/signal.h>
#define BUFF_LEN 50
#define WRITER_MIN 10
#define WRITER_RANGE 11
#define READER_MIN 10
#define READER_RANGE 11

typedef struct reqmem{
    sem_t write, mutex;
    int readercnt;
    char buffer[BUFF_LEN];
}required_memory;

int total, id, count;

int main(int argc, char** argv){
    total = 0;
    if (argc != 2){
        fprintf(stderr, "Invalid Arguments!\nUsage:%s <reader_count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int r = atoi(argv[1]);
    required_memory *shared = mmap(NULL, sizeof(required_memory), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (shared == MAP_FAILED){
        fprintf(stderr, "Memory map failed\n");
        exit(EXIT_FAILURE);
    }
    strncpy(shared->buffer, "Initial buffer line.", BUFF_LEN);
    shared->readercnt = 0;
    sem_t *write, *mutex;
    write = &(shared->write);
    mutex = &(shared->mutex);
	sem_init(write, 1, 1);
	sem_init(mutex, 1, 1);
	pid_t readers[r], writer;
    int i;
    for (i = 0; i < r; i++){
        pid_t pid = fork();
        if (pid < 0){
            goto __fork_error_handler_r;
        }
        else if (pid){
            readers[i] = pid;
        }
        else{
            id = i;
            goto __reader;
        }        
    }
    writer = fork();
    if(writer < 0){
        goto __fork_error_handler_w;
    }else if(!writer){
        goto __writer;
    }
    while(i--)
        wait(NULL);
    wait(NULL);
    exit(EXIT_SUCCESS);

    __fork_error_handler_r: {
        fprintf(stderr, "Some error while forking readers!\n");
        for(--i; i>=0; i--){
            kill(readers[i], SIGKILL);
        }
        exit(EXIT_FAILURE);
    } 
    __fork_error_handler_w: {
        fprintf(stderr, "Some error while forking writer!\n");
        for(--i; i>=0; i--){
            kill(readers[i], SIGKILL);
        }
        exit(EXIT_FAILURE);
    } 
    __writer: {
        srand(time(NULL));
        count = WRITER_MIN + rand() % WRITER_RANGE;
        int c = count;
        int line = 0;
        while(c--){
            struct timespec sltime = {0, rand()%150 << 10};
            nanosleep(&sltime, NULL);
		    line++;
		    char buff[BUFF_LEN];
		    sem_wait(write);
            sprintf(buff, "This is line number %d written by the writer.", line);
		    strncpy(shared->buffer, buff, BUFF_LEN);
		    sem_post(write);
            sleep(1);
        }
        printf("Writer wrote %d lines.\n", count);
	    return 0;
    } 
    __reader: {
        srand(time(NULL));
	    int l = READER_MIN + rand() % READER_RANGE;
	    while (l--){
		    sem_wait(mutex);
            shared->readercnt++;
		    if (shared->readercnt == 1){
                sem_wait(write);
            }		    
		    sem_post(mutex);
            count++;
		    printf("Line %d read by reader %d: %s\n", count, id+1, shared->buffer);
		    sem_wait(mutex);
		    shared->readercnt--;
		    if (shared->readercnt == 0){
                sem_post(write);
            }
		    sem_post(mutex);
            sleep(1);
	    }
	    printf("Reader %d read %d lines.\n", id+1, count);
	    return 0;
    }
}
