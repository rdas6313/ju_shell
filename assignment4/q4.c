#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#define MAX 1000
int main(){
	int option;
	char *args[MAX];

	while(1){
		memset(args,'\0',sizeof(args));
		printf("1. ls\n");
		printf("2. pwd\n");
		printf("3. uname\n");
		printf("4. exit\n");
		printf("> ");
		scanf("%d",&option);
		switch(option){
			case 1:
				args[0] = "/bin/ls";
				break;
			case 2:
				args[0] = "/bin/pwd";
				break;
			case 3:
				args[0] = "/usr/bin/uname";
				break;
			case 4:
				return 0;
		}
		int pid = fork();
		if(pid < 0){
			printf("Error: unable to create child process\n");
			return 0;
		}else if(pid == 0){
			execv(args[0],args);
			printf("Error: unable to load new program\n");
			return 0;
		}else{
			wait(NULL);
		}

	}

	return 0;
}