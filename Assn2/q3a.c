#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#define READ_END 0
#define WRITE_END 1
#define BUFF_LEN 50
int flag = 1;

void __listener_handler(int sig){
    if (sig == SIGINT){
        return;
    }
    if(sig == SIGTERM){
        exit(EXIT_SUCCESS);
    }    
}

void __broadcast_signal_handler(int sig){
    flag = 0;
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("%s: Invalid Arguments\nUsage: %s <station_count>\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    pid_t stations[n];
    int fds[n][2];
    int id;
    char buffer[BUFF_LEN];
    for (id = 0; id < n; id++){
        pipe(fds[id]);
        if(stations[id] = fork()){
            goto __listen;
        }else{
            close(fds[id][READ_END]);
        }
    }
    signal(SIGINT, __broadcast_signal_handler);
    srand(time(NULL));
    while (flag) {
        sleep(rand() % 2);
        time_t t;
		time(&t);
		struct tm *time_val = localtime(&t);
		char *arr[] = {"Thunder storm", "Clear sky", "Snowfall", "Overcast", "Haze", "Drizzles"};
		sprintf(buffer, "%2d:%2d:%2d The weather condition is: %s.", (time_val->tm_hour) % 12, time_val->tm_min,time_val->tm_sec, arr[rand() % 6]);
		int l = strlen(buffer) + 1;
        for (int i = 0; i < n; i++){
            write(fds[i][WRITE_END], buffer, l);
        }
        
    }
    for (int i = 0; i < n; i++){
        kill(stations[i], SIGTERM);
    }
    sleep(1);
    printf("Forecast end.");
    fflush(stdout);
    return 0;

    __listen: {
        signal(SIGINT, __listener_handler);
        signal(SIGTERM, __listener_handler);
        for (int i = 0; i < id; i++){
            close(fds[i][WRITE_END]);
        }
        int fd = fds[id][READ_END];
        while (id+1){
            read(fd, buffer, BUFF_LEN);
            printf("Station %d: %s\n", id+1, buffer);
            id--;
        }        
    }    
    
}
