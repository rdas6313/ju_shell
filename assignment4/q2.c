/*Question 2 of assignment 4 */
#include <unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<math.h>
#define LIMIT 10

int is_prime(int num){
    if(num < 0)
        return -1;
    int i,sqr = sqrt(num);

    for(i=2;i<=sqr;i++){
        if(num % i == 0)
            return 0;
    }
    return 1;
}

void generatePrime(int start,int end){
    if(start > end){
        perror("Error: start is greater than end\n");
        return;
    }
    while(start <= end){
        if(is_prime(start))
            printf("Process %d: %d\n",getpid(),start);
        start++;
    }
}

int main(){
    int start = 1,end = LIMIT,i;
    printf("Printing prime no 1 to %d using 10 different processes\n",LIMIT*10);
    for(i=0;i<10;i++){
        pid_t pid = fork();
        if(pid < 0){
            perror("Error: unable to create process\n");
            return 0;
        }else if(pid > 0){
            //parent process
            int tmp = end + LIMIT;
            start = end+1;
            end = tmp;
        }else{
            //child process
            generatePrime(start,end);
            return 0;
        }
    }
    printf("Waiting for child to complete\n");
    wait(NULL);
    printf("Exiting parent process\n");
    return 0;
}