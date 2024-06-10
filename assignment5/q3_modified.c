// Question 3 of assignment 5 (Producer and consumer)

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#define MAX 10
#define FREE_SEM 1
#define ITEM_SEM 2
#define SHARED_SEM 0

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

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

int getSValue(int semid, int index)
{
    int value = semctl(semid, index, GETVAL);
    if (value == -1)
    {
        printf("Error: unable to get semaphore value\n");
    }
    return value;
}

void producer_process(int *shared, int semid)
{
    printf("Press enter to produce item\n");
    int *front = shared;
    int *rear = (shared + 1);
    shared = (rear + 1);
    int semValue;
    int item = 0;
    *rear = -1;
    while (1)
    {
        getchar();
        if (!getSValue(semid, FREE_SEM))
        {
            printf("Buffer Full! Can't produce\n");
        }
        waits(semid, FREE_SEM);
        waits(semid, SHARED_SEM);
        *rear = (*rear + 1) % MAX;
        shared[*rear] = item++;
        printf("Producing item %d at %d\n", shared[*rear], *rear);
        signals(semid, SHARED_SEM);
        signals(semid, ITEM_SEM);
    }
}

void consumer_process(int *shared, int semid)
{
    int *front = shared;
    int *rear = (shared + 1);
    shared = (rear + 1);
    int semValue;
    int item = 0;
    *front = 0;
    while (1)
    {
        if (!getSValue(semid, ITEM_SEM))
        {
            printf("Empty Buffer! Can't consume\n");
        }
        waits(semid, ITEM_SEM);
        waits(semid, SHARED_SEM);
        item = shared[(*front)];
        printf("Consumig item %d at %d\n", item, *front);
        *front = ((*front) + 1) % MAX;
        signals(semid, SHARED_SEM);
        signals(semid, FREE_SEM);
        sleep(1);
    }
}

int main()
{

    const char *path = "/Users/rajadas/Documents/ju_shell/assignment5/tmp";
    int project_id = 1012231;

    key_t key;
    if ((key = ftok(path, project_id)) == -1)
    {
        printf("Error: unable to generate unique key.Create new path and change project id.\n");
        return 0;
    }

    int size = sizeof(int) * 12;
    int shareid = shmget(key, size, IPC_CREAT | 0666);
    if (shareid < 0)
    {
        printf("Error: unable to create shared memory id\n");
        return 0;
    }

    int *shared = shmat(shareid, NULL, 0);

    int semid;
    if ((semid = semget(key, 3, IPC_CREAT | 0666)) == -1)
    {
        printf("Error: unable to create semaphore\n");
        return 0;
    }

    unsigned short sem_values[3] = {1, 10, 0};
    union semun semopts;
    semopts.array = sem_values;

    if (semctl(semid, 0, SETALL, semopts) == -1)
    {
        printf("Error: unable to set semaphore values\n");
        return 0;
    }

    int pid = fork();
    if (pid < 0)
    {
        printf("Error: unable to create producer process\n");
        return 0;
    }
    else if (pid == 0)
    {
        producer_process(shared, semid);
    }
    else
    {
        // parent process
        int pid2 = fork();
        if (pid2 < 0)
        {
            printf("Error: unable to create consumer process\n");
            return 0;
        }
        else if (pid2 == 0)
        {
            consumer_process(shared, semid);
        }
        else
        {
            wait(NULL);
        }
    }
    return 0;
}
