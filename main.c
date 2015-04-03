#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "semun.h"

#define SHMSZ   4

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main(int argc, char* argv[])
{
    key_t key = 5678;
    int shmid;
    int* shm;
    int contribution = 0;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }   

    if ((shm = shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }   

    *shm = 0;

	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	if (!set_semvalue()) {
		fprintf(stderr, "Failed to initialize semaphore\n");
		exit(EXIT_FAILURE);
	}

    pid_t pid = getpid();

	fork();
    fork();

	pid_t current_pid = getpid();

    while (*shm < 10000) {
        ++contribution;
		if (!semaphore_p()) exit(EXIT_FAILURE);
        ++(*shm);
		if (!semaphore_v()) exit(EXIT_FAILURE);
        printf("%d\n", *shm);
    }   

    printf("%s : %d/%d\n", pid == 0?"server":"client", contribution, *shm);

	if (pid == current_pid)
		del_semvalue();

    exit(0);
}

static int set_semvalue(void)
{
	union semun sem_union;

	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return 0;

	return 1;
}

static void del_semvalue(void)
{
	union semun sem_union;

	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}

static int semaphore_p(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n");
		return 0;
	}

	return 1;
}

static int semaphore_v(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
		return 0;
	}

	return 1;
}

