/* Question 1 of assignment 4 */
#include <unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>
void printIds(int isParent){
    int status;
    if(!isParent){
        printf("Child Process:\n");
        printf("Enter a key!\n");
        getchar();
        printf("Child process id: %d\n",getpid());
        printf("Child process parent id: %d\n",getppid());

    }
    else{
        printf("Parent Waiting: \n");
        pid_t child_pid = wait(&status);
        printf("Parent process:\n");
        printf("Exit status of child %d: %d\n",child_pid,status);
        printf("Parent process id: %d\n",getpid());
        printf("Parent process parent id: %d\n",getppid());

    }
        
}
int main(){
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("Error: Unable to create child process");
        break;    
    default:
        printIds(pid);
    }
}
