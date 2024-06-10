#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <string.h>
#define READER_SEM 0
#define WRITER_SEM 1
#define NO_OF_WRITE_PROCESS 2
#define NUM_OF_READ_PROCESS 2

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

void writer_process(int semid, int *shared)
{
    int pid = getpid();
    shared++;
    printf("Press enter to write to shared memory\n");
    while (1)
    {
        getchar();
        waits(semid, WRITER_SEM);
        (*shared)++;
        printf("Writing to shared memory: %d by writer process %d\n", *shared, pid);
        signals(semid, WRITER_SEM);
    }
}

void reader_process(int semid, int *shared)
{
    int pid = getpid();
    int *reader_count = shared;
    shared++;
    while (1)
    {
        waits(semid, READER_SEM);
        (*reader_count)++;
        if (*reader_count == 1)
        {
            signals(semid, READER_SEM);
            waits(semid, WRITER_SEM);
        }
        else
            signals(semid, READER_SEM);

        printf("Reading from shared memory: %d by reader process %d\n", *shared, pid);

        waits(semid, READER_SEM);
        (*reader_count)--;
        if (*reader_count == 0)
        {
            signals(semid, READER_SEM);
            signals(semid, WRITER_SEM);
        }
        else
            signals(semid, READER_SEM);
        sleep(2);
    }
}

int main()
{

    const char *path = "/Users/rajadas/Documents/ju_shell/assignment5/tmp";
    int project_id = 10123999;

    key_t key;
    if ((key = ftok(path, project_id)) == -1)
    {
        printf("Error: unable to generate unique key.Create new path and change project id.\n");
        return 0;
    }

    int n = 2;

    int shareid = shmget(key, n * sizeof(int), IPC_CREAT | 0666);
    if (shareid < 0)
    {
        printf("Error: unable to create shared memory id\n");
        return 0;
    }

    int *shared = shmat(shareid, NULL, 0);
    memset(shared, 0, sizeof(int) * n);

    int semid;
    if ((semid = semget(key, 2, IPC_CREAT | 0666)) == -1)
    {
        printf("Error: unable to create semaphore\n");
        return 0;
    }

    unsigned short sem_values[2] = {1, 1};
    union semun semopts;
    semopts.array = sem_values;

    if (semctl(semid, 0, SETALL, semopts) == -1)
    {
        printf("Error: unable to set semaphore values\n");
        return 0;
    }

    int readProcess = NUM_OF_READ_PROCESS;
    while (readProcess > 0)
    {
        int pid = fork();
        if (pid < 0)
        {
            printf("Error: unable to create writer process\n");
            return 0;
        }
        else if (pid != 0)
        {
            readProcess--;
            continue;
        }
        else
        {
            reader_process(semid, shared);
            return 0;
        }
    }

    int writerProcess = NO_OF_WRITE_PROCESS;
    while (writerProcess > 0)
    {
        int pid = fork();
        if (pid < 0)
        {
            printf("Error: unable to create writer process\n");
            return 0;
        }
        else if (pid != 0)
        {
            writerProcess--;
            continue;
        }
        else
        {
            writer_process(semid, shared);
            return 0;
        }
    }

    wait(NULL);

    return 0;
}