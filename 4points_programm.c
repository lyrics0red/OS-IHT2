#include "sys/mman.h"
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char** argv) {
    int time = atoi(argv[3]);
    char* memn = "shared-memory";
    int shm;
    int mem_size = 4096;
    shm = shm_open(memn, O_CREAT|O_RDWR, 0666);
    ftruncate(shm, mem_size);
    int** field = (int**)mmap(0, 4096, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    field = (int**)malloc(30 * sizeof(int*));
    for (int i = 0; i < 30; ++i) {
        field[i] = (int*)malloc(30 * sizeof(int));
    }
    int count = rand() % 180 + 90;
    for (int i = 0; i < count; ++i) {
        int x = rand() % 30, y = rand() % 30;
        field[x][y] = 3;
    }
    char* first = "first";
    char* second = "second";
    sem_t* firstSem = sem_open(first, O_CREAT, 0666, 1);
    sem_t* secondSem = sem_open(second, O_CREAT, 0666, 0);
    int result = fork();
    if (result > 0) {
    	for (int i = 0; i < 30; ++i) {
    	    if (i % 2 == 0) {
    	        for (int j = 0; j < 30; ++j) {
    	            sem_wait(firstSem);
    	            if (field[i][j] != 2 && field[i][j] != 3) {
    	                //sleep();
    	                field[i][j] = 1;
    	            } else {
    	                //sleep();
    	            }
    	            sem_post(secondSem);
    	        }
    	    } else {
    	        for (int j = 29; j >= 0; --j) {
    	            sem_wait(firstSem);
    	            if (field[i][j] != 2 && field[i][j] != 3) {
    	                //sleep();
    	                field[i][j] = 1;
    	            } else {
    	                //sleep();
    	            }
    	            sem_post(secondSem);
    	        }
    	    }
        }
    } else {
        for (int i = 29; i >= 0; --i) {
    	    if (i % 2 == 0) {
    	        for (int j = 0; j < 30; ++j) {
    	            sem_wait(secondSem);
    	            if (field[j][i] != 1 && field[j][i] != 3) {
    	                //sleep();
    	                field[j][i] = 2;
    	            } else {
    	                //sleep();
    	            }
    	            sem_post(firstSem);
    	        }
    	    } else {
    	        for (int j = 29; j >= 0; --j) {
    	            sem_wait(secondSem);
    	            if (field[j][i] != 1 && field[j][i] != 3) {
    	                //sleep();
    	                field[j][i] = 2;
    	            } else {
    	                //sleep();
    	            }
    	            sem_post(firstSem);
    	        }
    	    }
        }
        sem_close(firstSem);
        sem_close(secondSem);
        sem_unlink(first);
        sem_unlink(second);
    }
    for (int i = 0; i < 30; ++i) {
            for (int j = 0; j < 30; ++j) {
                printf("%d", field[i][j]);
            }
            printf("\n");
    }
}
