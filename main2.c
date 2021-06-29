#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
	int status;
	int pp1[2];
	int pp2[2];
	int pp3[2];
	int i = 0;
	pid_t pid;

	pipe(pp1);
	pid = fork();
	if (pid == 0)
	{
		close(pp1[0]);
		dup2(pp1[1], 1);
		close(pp1[1]);
		execlp("/bin/cat", "cat", NULL);
	}
	else
	{
		close(pp1[1]);
		pipe(pp2);
		pid = fork();
		if (pid == 0)
		{
			close(pp2[0]);

			dup2(pp1[0], 0);
			close(pp1[0]);
			dup2(pp2[1], 1);
			close(pp2[1]);
			execlp("/bin/cat", "cat", NULL);
		}
		else
		{
			close(pp1[0]);
			close(pp2[1]);
			pipe(pp3);
			pid = fork();
			if (pid == 0)
			{
				close(pp3[0]);
				dup2(pp2[0], 0);
				close(pp2[0]);
				dup2(pp3[1], 1);
				execlp("/bin/cat", "cat", NULL);
			}
			else
			{
				close(pp2[0]);
				close(pp3[1]);
				pid = fork();
				if (pid == 0)
				{
					dup2(pp3[0], 0);
					close(pp3[0]);
					execlp("/bin/ls", "ls", NULL);
				}
				close(pp3[0]);
				while (i < 4)
				{
					++i;
					waitpid(-1, &status, 0);
				}
			}
		}
	}

	return 0;
}