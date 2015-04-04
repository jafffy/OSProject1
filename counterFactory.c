#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

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

	return 0;
}
