#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define N 7
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + N-1) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N];
sem_t *mutex;
sem_t S[N];
void test(int phnum){
	if (state[phnum] == HUNGRY && state[LEFT] != EATING	&& state[RIGHT] != EATING) {
		state[phnum] = EATING;
		sleep(1);
		printf("Philosopher %d takes chopstick %d and %d\n",	phnum + 1, LEFT + 1, phnum + 1);
		printf("Philosopher %d is Eating\n", phnum + 1);
		sem_post(&S[phnum]);
	}
}
void take_chopstick(int phnum){
	sem_wait(mutex);
	state[phnum] = HUNGRY;
	printf("Philosopher %d is Hungry\n", phnum + 1);
	test(phnum);
	sem_post(mutex);
	sem_wait(&S[phnum]);
	sleep(1);
}
void put_chopstick(int phnum){
	sem_wait(mutex);
	state[phnum] = THINKING;
	printf("Philosopher %d putting chopstick %d and %d down\n", phnum + 1, LEFT + 1, phnum + 1);
	printf("Philosopher %d is thinking\n", phnum + 1);
	test(LEFT);
	test(RIGHT);
	sem_post(mutex);
}
void* philosopher(void* num){
	while (true) {
		int* i = num;
		sleep(1);
		take_chopstick(*i);
		sleep(0);
		put_chopstick(*i);
	}
}
int main(){
    for(int i=0; i<N; i++){
        phil[i] = i;
    }
	int i;
	pthread_t thread_id[N];
	sem_init(mutex, 0, 1);
	for (i = 0; i < N; i++)
		sem_init(&S[i], 0, 0);
	for (i = 0; i < N; i++) {
		pthread_create(&thread_id[i], NULL,	philosopher, &phil[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}
	for (i = 0; i < N; i++)
		pthread_join(thread_id[i], NULL);
}
