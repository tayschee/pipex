#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
	int i;
	char buf[10] = {0};
	pid_t pid;
	int pipefd[2];
	int pipefd2[2];
	int status;
	int fdin;

	if ((pid = fork()) < 0)
	{
		printf("error\n");
		return 1;
	}
	if (pid == 0)
	{
		if (pipe(pipefd) < 0)
			return 1;
		if ((pid = fork()) < 0)
		{
			printf("error\n");
			return 1;
		}
		if (pid == 0)
		{
			if ((pid = fork()) < 0)
			{
				printf("error\n");
				return 1;
			}
			if (pid == 0)
			{
				sleep(10);
				close(pipefd[1]);
				dup2(pipefd[0], 0);
				dup2(pipefd[1], 1);
				execl("/bin/echo", "echo", "\n\n", NULL);
			}
			else
			{
				//dup2(pipefd[1], 1);
				dup2(pipefd[0], 0);
				execl("usr/bin/grep", "grep", "a", NULL);
			}
		}
		else
		{
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			//close(pipefd[1]);
			execl("/bin/ls", "ls", NULL);
		}
	}
	else
	{
		waitpid(-1, &status, 0);
		waitpid(-1, &status, 0);
		waitpid(-1, &status, 0);
	}
	return 0;
}