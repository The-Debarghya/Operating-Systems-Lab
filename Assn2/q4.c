#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
/*#include <sys/mman.h>
#include <sys/shm.h>*/
#define BUFF_LEN 50
#define PER_PRODUCER_MIN 100
#define PER_PRODUCER_RANGE 101

typedef struct reqmem{
    int total, start, count;
    unsigned char buffer[BUFF_LEN];
    sem_t f, e, m;
}requiredmemory;

