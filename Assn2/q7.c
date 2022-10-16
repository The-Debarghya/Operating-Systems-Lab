#include <stdio.h>
#include <stdlib.h>
#define BUFF 20

int max[BUFF][BUFF];
int allocation[BUFF][BUFF];
int need[BUFF][BUFF];
int available[BUFF];
int request[BUFF];
int finish[BUFF];

int main(int argc, char** argv){
    if (argc != 2){
        fprintf(stderr, "Invalid Arguments!\nUsage:%s <input_filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int i, j, k, n, m, p, flag, count = 0;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resources: ");
    scanf("%d", &m);
    printf("Enter the allocation matrix:");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }
    printf("Enter the max matrix:");
    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }
    printf("Enter the available matrix:");
    for (i = 0; i < m; i++) {
        scanf("%d", &available[i]);
    }
    printf("Enter the request matrix:");
    for (i = 0; i < m; i++)  {
        scanf("%d", &request[i]);
    }
    for (i = 0; i < n; i++) {
        finish[i] = 0;
    }
    for (i = 0; i < n; i++)  {
        for (j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
    do {
        flag = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                p = 0;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > available[j]) {
                        p = 1;
                        break;
                    }
                }
                if (p == 0){
                    for (j = 0; j < m; j++) {
                        available[j] += allocation[i][j];
                    }
                    finish[i] = 1;
                    flag = 1;
                }
            }
        }
        count++;
    } while (flag != 0 && count < n);
    if (count < n){
        printf("The system is in safe state.\n");
    }
    else{
        printf("The system is in unsafe state.\n");
    }
    return 0;
}