#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define MAX 100
int parToch[2];
int chTopar[2];

int is_palidrome(char data[])
{
	int len = strlen(data);
	int i = 0, j = len - 1;
	while (i < j)
	{
		if (data[i] != data[j])
			return 0;
		i++;
		j--;
	}
	return 1;
}

int main()
{

	if (pipe(parToch) < 0 || pipe(chTopar) < 0)
	{
		printf("Error: Unable to create pipe\n");
		return 0;
	}

	int pid = fork();
	if (pid < 0)
	{
		printf("Error:Unable to create child\n");
		return 0;
	}
	else if (pid > 0)
	{
		// parent
		char data[MAX];
		printf("Enter a string: ");
		scanf("%s", data);

		// sending data to child
		close(parToch[0]);
		int bytes = write(parToch[1], data, strlen(data));
		if (bytes < 0)
		{
			printf("Error: unable to send data to child\n");
			return 0;
		}
		printf("Sent %d bytes data to child\n", bytes);
		// receiving data from child
		memset(data, '\0', sizeof(data));
		close(chTopar[1]);
		bytes = read(chTopar[0], data, MAX);
		if (bytes < 0)
		{
			printf("Error: Unable to receive data from child\n");
			return 0;
		}
		printf("Received %d bytes data from child\n", bytes);
		printf("Palindrome: %s\n", data);
	}
	else
	{
		char data[MAX];
		// receiving data from parent
		close(parToch[1]);
		int bytes = read(parToch[0], data, MAX);
		if (bytes < 0)
		{
			printf("Error: Unable to receive data from parent\n");
			return 0;
		}
		printf("Received %d bytes data from parent\n", bytes);
		// check for palindrome
		char response[5];
		memset(response, '\0', sizeof(response));
		if (is_palidrome(data))
		{
			strcpy(response, "YES");
		}
		else
		{
			strcpy(response, "NO");
		}
		// send data to parent
		close(chTopar[0]);
		bytes = write(chTopar[1], response, strlen(response));
		if (bytes < 0)
		{
			printf("Error: unable to send data to parent\n");
			return 0;
		}
		printf("Sent %d bytes data to parent\n", bytes);
	}
	return 0;
}