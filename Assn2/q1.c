#include "q1.h"

int time = 0;
int cpu = 0, io = 0;
job_queue unarrived, jobqueue, ioqueue;
int currentjobs[__MASK__][4] = {0};
int cpubusy = 0, iobusy = 0;
job currentcpujob, currentiojob;
int count = 0;
struct stat stats[1000];
job dummy = {-1, -1, -1, NULL};