#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <sys/wait.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

void printSemaphores(int semid, int index)
{
	if (index != -1)
	{
		int id = semctl(semid, index, GETVAL);
		if (id < 0)
		{
			printf("Error: unable to get semaphore values\n");
			return;
		}
		printf("Index %d: %d\n", index, id);
		return;
	}
	unsigned short sem_values[3];
	int id = semctl(semid, 0, GETALL, sem_values);
	if (id < 0)
	{
		printf("Error: unable to get semaphore values\n");
		return;
	}
	int i;
	for (i = 0; i < 3; i++)
		printf("%d ", sem_values[i]);
	printf("\n");
}

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
	// printSemaphores(semid,-1);
	// printf("Calling wait on %d\n",index);
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
	// printSemaphores(semid,-1);

	return val;
}

void child1(int semid)
{
	// printf("Starting process p1\n");
	// printSemaphores(semid,-1);
	while (1)
	{
		waits(semid, 0);
		printf("A");
		fflush(stdout);
		sleep(1);
		signals(semid, 1);
	}
}

void child2(int semid)
{
	// printf("Starting process p2\n");
	// printSemaphores(semid,-1);

	int tmp = 0;
	while (1)
	{
		waits(semid, 1);
		printf("B");
		fflush(stdout);
		sleep(1);
		if (tmp == 0)
		{
			tmp = 1;
			signals(semid, 2);
		}
		else
		{
			tmp = 0;
			printf("\n");
			signals(semid, 0);
		}
	}
}

void child3(int semid)
{

	// printf("Starting process p3\n");
	// printSemaphores(semid,-1);
	int tmp = 0;
	while (1)
	{
		waits(semid, 2);
		sleep(1);
		printf("C");
		fflush(stdout);
		if (tmp == 0)
		{
			tmp = 1;
			signals(semid, 2);
		}
		else
		{
			tmp = 0;
			signals(semid, 0);
		}
	}
}

int main()
{

	const char *path = "/Users/rajadas/Documents/ju_shell/assignment5/tmp";
	int project_id = 10122002;

	key_t key;
	if ((key = ftok(path, project_id)) == -1)
	{
		printf("Error: unable to generate unique key. Create valid path and change project id.\n");
		return 0;
	}

	int semid;
	if ((semid = semget(key, 3, IPC_CREAT | 0666)) == -1)
	{
		printf("Error: unable to create semaphore\n");
		return 0;
	}

	unsigned short sem_values[3] = {1, 0, 0};
	union semun semopts;
	semopts.array = sem_values;

	if (semctl(semid, 0, SETALL, semopts) == -1)
	{
		printf("Error: unable to set semaphore values\n");
		return 0;
	}
	// printSemaphores(semid,-1);
	int pid1 = fork();
	if (pid1 < 0)
	{
		printf("Error: unable to create process 1\n");
		return 0;
	}
	else if (pid1 > 0)
	{
		int pid2 = fork();
		if (pid2 < 0)
		{
			printf("Error: unable to create process 2\n");
			return 0;
		}
		else if (pid2 > 0)
		{
			int pid3 = fork();
			if (pid3 < 0)
			{
				printf("Error: unable to create process 2\n");
				return 0;
			}
			else if (pid3 > 0)
			{
				// printf("Main Process: Created all three processes\n");
				wait(NULL);
			}
			else
			{
				child3(semid);
			}
		}
		else
		{
			child2(semid);
		}
	}
	else
	{
		child1(semid);
	}
	return 0;
}