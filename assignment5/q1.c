#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#define MAX_DATA 5
#define UPPER_LIMIT 10
#define LOWER_LIMIT 1

// union semun
// {
// 	int val;
// 	struct semid_ds *buf;
// 	unsigned short *array;
// };

int waits(int semid, int index)
{
	struct sembuf sem_op;
	sem_op.sem_num = index;
	sem_op.sem_op = -1;
	sem_op.sem_flg = 0;
	int val = semop(semid, &sem_op, 1);
	if (val < 0)
	{
		printf("Error: unable wait on index %d\n", index);
	}
	return val;
}

int signals(int semid, int index)
{
	struct sembuf sem_op;
	sem_op.sem_num = index;
	sem_op.sem_op = 1;
	sem_op.sem_flg = 0;
	int val = semop(semid, &sem_op, 1);
	if (val < 0)
	{
		printf("Error: unable to signal on index %d\n", index);
	}
	return val;
}

int rand_generator(int data, int upper, int lower)
{
	int size = upper - lower + 1;
	return lower + (data % size);
}

int main()
{

	size_t size = sizeof(int) * MAX_DATA;
	int *data = NULL;

	srand((unsigned int)time(NULL));

	const char *path = "/Users/rajadas/Documents/ju_shell/assignment5";
	int project_id = 10123991;

	key_t key;
	if ((key = ftok(path, project_id)) == -1)
	{
		printf("Error: unable to generate unique key\n");
		return 0;
	}

	int id = shmget(key, size, 0666 | IPC_CREAT);
	if (id < 0)
	{
		printf("Error: unable to create shared memory\n");
		return 0;
	}

	data = shmat(id, NULL, 0);
	if (data == (void *)-1)
	{
		printf("Error: unable to attach shared memory\n");
		return 0;
	}

	int semid;
	if ((semid = semget(key, 2, IPC_CREAT | 0666)) == -1)
	{
		printf("Error: unable to create semaphore\n");
		return 0;
	}

	unsigned short sem_values[3] = {1, 0};
	union semun semopts;
	semopts.array = sem_values;

	if (semctl(semid, 0, SETALL, semopts) == -1)
	{
		printf("Error: unable to set semaphore values\n");
		return 0;
	}

	int forkid = fork();
	if (forkid < 0)
	{
		printf("Error: unable to fork\n");
		return 0;
	}
	else if (forkid == 0)
	{
		// child process
		waits(semid, 1);

		double avg = data[0];
		int max = data[0], min = data[0], i;
		for (i = 1; i < MAX_DATA; i++)
		{
			max = data[i] > max ? data[i] : max;
			min = data[i] < min ? data[i] : min;
			avg += data[i];
		}
		avg = avg / MAX_DATA;

		data[0] = max;
		data[1] = min;
		double *avgData = (double *)(data + 2);
		*avgData = avg;

		signals(semid, 0);
	}
	else
	{
		// parent process
		waits(semid, 0);
		int i;
		printf("Numbers: ");
		for (i = 0; i < MAX_DATA; i++)
		{
			int tmp = rand_generator(rand(), UPPER_LIMIT, LOWER_LIMIT);
			*(data + i) = tmp;
			printf("%d ", tmp);
		}
		printf("\n");
		signals(semid, 1);

		waits(semid, 0);

		printf("Maximum: %d\n", *(data));
		printf("Minimum: %d\n", *(data + 1));
		printf("Avg: %lf\n", *(double *)(data + 2));
	}
}