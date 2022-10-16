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
    sem_t rentry, wentry, write, mut1, mut2;
    int readcnt, writecnt;
    char buffer[BUFF_LEN];
}required_memory;

int total, count, id;
int main(int argc, char** argv){
    total = 0;
    if (argc != 3){
        fprintf(stderr, "Invalid Arguments!\nUsage:%s <reader_count> <writer_count>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int R, W;
    R = atoi(argv[1]);
    W = atoi(argv[2]);
    required_memory *shared = mmap(NULL, sizeof(required_memory), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	strncpy(shared->buffer, "Initial buffer line", BUFF_LEN);
	shared->readcnt = 0;
	shared->writecnt = 0;
	sem_t *rentry, *wentry, *mut1, *mut2, *wrt;
	rentry = &(shared->rentry);
	wentry = &(shared->wentry);
	wrt = &(shared->write);
	mut1 = &(shared->mut1);
	mut2 = &(shared->mut2);
	sem_init(rentry, 1, 1);
	sem_init(wentry, 1, 1);
	sem_init(wrt, 1, 1);
	sem_init(mut1, 1, 1);
	sem_init(mut2, 1, 1);
	pid_t readers[R], writers[W];
	int r, w;
	for (r = 0; r < R; r++){
		pid_t pid = fork();
		if (pid < 0)
			goto __fork_error_handler_r;
		else if (pid){
			readers[r] = pid;
		}
		else{
			id = r;
			goto __reader;
		}
	}
	for (w = 0; w < W; w++)	{
		pid_t pid = fork();
		if (pid < 0)
			goto __fork_error_handler_w;
		else if (pid){
			writers[w] = pid;
		}
		else{
			id = w;
			goto __writer;
		}
	}
	while (r--)
		wait(NULL);
	while (w--)
		wait(NULL);
	exit(EXIT_SUCCESS);

    __fork_error_handler_r:{
	    fprintf(stderr, "Some error forking readers\n");
	    for (--r; r >= 0; r--)
		    kill(readers[r], SIGKILL);
	    exit(EXIT_FAILURE);
    }
    __fork_error_handler_w:{
	    fprintf(stderr, "Some error forking writers\n");
	    for (--r; r >= 0; r--)
		    kill(readers[r], SIGKILL);
	    for (--w; w >= 0; w--)
		    kill(writers[w], SIGKILL);
	    exit(EXIT_FAILURE);
    }
    __writer:{
	    srand(time(NULL));
	    count = WRITER_MIN + rand() % WRITER_RANGE;
	    int cnt = count;
	    int l = 0;
	    while (cnt--){
		    l++;
		    char buff[BUFF_LEN];
		    sprintf(buff, "Writer: %d Line: %d", id+1, l);
		    sem_wait(mut2);
		    shared->writecnt++;
		    if (shared->writecnt == 1)
			    sem_wait(rentry);
		    sem_post(mut2);
		    sem_wait(wrt);
		    strncpy(shared->buffer, buff, BUFF_LEN);
		    sem_post(wrt);
		    sem_wait(mut2);
		    shared->writecnt--;
		    if (shared->writecnt == 0)
			    sem_post(rentry);
		    sem_post(mut2);
            sleep(1);
	    }
	    printf("Writer %d wrote %d lines finished at %ld\n", id+1, count, clock());
	    return 0;
    }
    __reader:{
	    srand(time(NULL));
	    int l = READER_MIN + rand() % READER_RANGE;
	    while (l--)	{
		    count++;
		    sem_wait(rentry);
		    sem_wait(mut1);
		    if (!shared->readcnt)
			    sem_wait(wrt);
		    sem_post(rentry);
		    shared->readcnt++;
		    sem_post(mut1);
		    printf("Line: %d read by Reader: %d: '%s'\n", count, id+1, shared->buffer);
		    sem_wait(mut1);
		    shared->readcnt--;
		    if (!shared->readcnt)
			    sem_post(wrt);
		    sem_post(mut1);
            sleep(1);
	    }
	    printf("Reader: %d read %d lines finished at %ld\n", id+1, count, clock());
	    return 0;
    }
}