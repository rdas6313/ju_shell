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

void producer_process(int *free, int semid)
{
    printf("Press enter to produce item\n");
    while (1)
    {
        getchar();
        waits(semid, FREE_SEM);
        waits(semid, SHARED_SEM);
        printf("Producer: producing item\n");
        (*free)--;
        printf("Producer: total item %d\n", (MAX - *free));
        signals(semid, SHARED_SEM);
        signals(semid, ITEM_SEM);
    }
}

void consumer_process(int *free, int semid)
{
    while (1)
    {
        waits(semid, ITEM_SEM);
        waits(semid, SHARED_SEM);
        printf("Consumer: consuming item\n");
        (*free)++;
        printf("Consumer: total item %d\n", (MAX - *free));
        signals(semid, SHARED_SEM);
        signals(semid, FREE_SEM);
        sleep(1);
    }
}

int main()
{

    const char *path = "/home/rdas6313/Documents/ju_shell/assignment5/tmp";
    int project_id = 10123991;

    key_t key;
    if ((key = ftok(path, project_id)) == -1)
    {
        printf("Error: unable to generate unique key\n");
        return 0;
    }

    int shareid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    if (shareid < 0)
    {
        printf("Error: unable to create shared memory id\n");
        return 0;
    }

    int *shared = shmat(shareid, NULL, 0);
    *shared = MAX;

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