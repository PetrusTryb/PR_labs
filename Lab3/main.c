#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <sys/wait.h>

void setProcessName(int number){
	char* name = malloc(3);
	if(name==NULL)
		return;
	sprintf(name, "%d", number);
	prctl(PR_SET_NAME, name);
	free(name);
}

int main(int argc, char *argv[])
{
	int depth;
	if (argc < 2)
	{
		printf("Missing parameter: N\n");
		return 1;
	}
	depth = atoi(argv[1]);
	if(depth>20){
		printf("Parameter N out of range (0-20)\n");
		return 1;
	}
	int pid,waiting_for;
	for (int i = 1; i <= depth; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("Fork error");
			return 1;
		}
		if (pid == 0)
		{
			setProcessName(i);
			if (i % 2 == 1)
				continue;
			return 0;
		}
		if (i % 2 == 1)
			waiting_for = pid;
		pid = fork();
		if (pid < 0)
		{
			perror("Fork error");
			return 1;
		}
		if (pid == 0)
		{
			setProcessName(i);
			if (i % 2 == 0)
				continue;
			return 0;
		}
		if (i % 2 == 0)
			waiting_for = pid;
		waitpid(waiting_for, NULL, 0);
		return 0;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Fork error");
		return 1;
	}
	if (pid == 0)
	{
		execlp("ps", "-u $USER", "--forest", NULL);
		return 0;
	}
	else
		waitpid(pid, NULL, 0);
	return 0;
}
