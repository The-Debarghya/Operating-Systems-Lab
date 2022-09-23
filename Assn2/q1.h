#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define __MASK__ 127
#ifndef Q1_H
#define Q1_H
struct burststruct{
    int time;
    struct burststruct* next;
};

typedef struct jobstruct{
    int jobid, priority, arrival;
    struct burststruct* burstval;
}job;

struct jobnode{
    job j;
    struct jobnode* next;
};

typedef struct jqueue{
    struct jobnode *head, *tail;
}job_queue;

typedef enum{
    job_arrival,
    cpuend,
    ioend,
    job_done
}_event;

typedef struct st{
	int jobid, arrival, finish, wait, priority;
}stat;

extern int time;
extern int cpu, io;
extern job_queue unarrived, jobqueue, ioqueue;
extern int currentjobs[__MASK__][4];
extern int cpubusy, iobusy;
extern job currentcpujob, currentiojob;
extern int count;
extern stat stats[1000];
extern job dummy;

#pragma region queueing_operations

extern void init_q(job_queue *q);
extern void enqueue(job_queue *q, job j);
extern int empty(job_queue q);
extern job front(job_queue q);
extern void dequeue(job_queue *q);

#pragma endregion
extern int parse_jobs();
extern void store_stats(int id);
extern void display_stats();

#endif