#include "pipex.h"
#include <stdio.h>

static void print_cmd(cmd *list_cmd)
{
	printf("list_cmd : %p\n\n", list_cmd);
	while (list_cmd != NULL)
	{
		printf("elem\n");
		int i = 0;
		while (list_cmd->cmd[i] != NULL)
		{
			printf("%s ", list_cmd->cmd[i]);
			++i;
		}
		printf("\n");
		printf("next : %p\n", list_cmd->next);
		printf("actual : %p\n", list_cmd);
		printf("prev : %p\n\n", list_cmd->prev);
		list_cmd = list_cmd->next;
	}
}

int    redirect_fd(int old_fd, int new_fd)
{
    int dup_fd;

    if ((dup_fd = dup(old_fd)) == -1)
        return -1;
    //close(old_fd);
    if (dup2(old_fd, new_fd) == -1)
    {
        dup2(old_fd, dup_fd);
        return -1;
    }
    return dup_fd;
}

void    parent_wait(int pid, cmd *list_cmd, int pipefd[2])
{
	(void)pipefd;
    int status;

    waitpid(pid, &status, 0);
}

void	pipe_gestion(cmd *list_cmd)
{
	if (list_cmd->prev == NULL)
	{
		dup2(list_cmd->pipefd[0], 1);
		close(list_cmd->pipefd[0]);
	}
	if (list_cmd->next == NULL)
	{
		close(list_cmd->pipefd[1]);
	}

	dup2(list_cmd->pipefd[0], 0);
	dup2(list_cmd->pipefd[1], 1);
}

void    child_execute(cmd *list_cmd, int first_fd, int last_fd, char **env, int pipefd[2])
{
	(void)env;
	(void)first_fd;
	(void)last_fd;

	if (list_cmd->prev == NULL)
		dup2(pipefd[1], 0);

	if (list_cmd->next != NULL)
		dup2(pipefd[1], 1);
	else
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	
	while (list_cmd)
	{
		print_cmd(list_cmd);
		pipe_gestion(list_cmd);
		if (execve(list_cmd->cmd[0], list_cmd->cmd, env) == -1)
		{

			printf("exec fail\n");
			exit(0);
		}
	}
}

int     execute(cmd *list_cmd, int first_fd, int last_fd, char **env)
{
	int pipefd[2];
    pid_t pid;

	printf("oooh\n");
	pid = fork();
	if (pid == -1)
	{
		return 1;
	}
	else if (pid == 0)
	{
		child_execute(list_cmd, first_fd, last_fd, env, pipefd);
	}
	else
	{
		parent_wait(pid, list_cmd, pipefd);
	}
	list_cmd = list_cmd->next;

    return 0;
}


static cmd *init_cmd(int c, char **v, cmd *prev)
{
	cmd *cmd_elem;

	if (c == 0)
		return NULL;

	if (!(cmd_elem = malloc(sizeof(cmd))))
		return NULL;
	if (!(cmd_elem->cmd = ft_split(v[0], ' ')))
	{
		return NULL;		
	}
	cmd_elem->prev = prev;
	cmd_elem->next = init_cmd(c - 1, &v[1], cmd_elem);
	return cmd_elem;
}

static int    pipex(int c, char **v, char **env)
{
	cmd *list_cmd;
	int first_fd = -1;
	int last_fd = -1;

	/*if ((first_fd = open(v[0], O_WRONLY)) == -1)
	{
		return 1;
	}*/
	/*if ((last_fd = open(v[c - 1], O_WRONLY)) == -1)
	{
		return 1;
	}*/

	//c -= 2;
	list_cmd = init_cmd(c, &v[0], NULL);

	execute(list_cmd, first_fd, last_fd, env);
	close(first_fd);
	close(last_fd);
	return 0;
}

int main(int c, char **v, char **env)
{
    (void)v;
    int ret;

    if (c < 2)
    {
        ft_putstr(ERROR_ARGUMENT);
        return 1;
    }
    else
    {
        ret = pipex(c - 1, &v[1], env);
		return ret;
    }
}