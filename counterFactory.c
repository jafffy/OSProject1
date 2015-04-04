#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <errno.h>

#include "semun.h"

#include "semaphore.h"

#define SHMSZ 4

int main(int argc, char* argv[])
{
	// Shared memory
	key_t key = 5678;
	int shmid; int* shm;

	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }   

    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }   

    *shm = 0;

    // Semaphore
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	if (!set_semvalue()) {
		fprintf(stderr, "Failed to initialize semaphore\n");
		exit(EXIT_FAILURE);
	}

	// File
	char buf[BUFSIZ] = { 0, };
	sprintf(buf, "timestamp%d", atoi(argv[1]));
	FILE* fp = fopen(buf, "at");

	struct timeval tv;
	gettimeofday(&tv, NULL);

	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

	int numberOfProcesses = pow(2, atoi(argv[1]));
	pid_t children[numberOfProcesses];
	int i;

	for (i = 0; i < numberOfProcesses; ++i) {
		pid_t pid = fork();

		if (pid < 0) {
			fprintf(stderr, "fork() failed\n");
		} else if (pid == 0) {
			sprintf(buf, "%d", numberOfProcesses);
			execlp("/home/vcl/workspace/CounterProcess/process", "process", buf, NULL);
		} else {
			children[i] = pid;
		}
	}

	int status;
	for (i = 0; i < numberOfProcesses; ++i) {
		waitpid(children[i], &status, 0);
	}

	struct timeval tv2;
	gettimeofday(&tv2, NULL);
	double current = (tv2.tv_sec) * 1000 + (tv2.tv_usec) / 1000;

	fprintf(fp, "%f\n", current - time_in_mill);

	// File
	fclose(fp);

	// Shared memory release
	if (shmdt(shm) == -1) {
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

	// Semaphore release
	del_semvalue();

	return 0;
}
