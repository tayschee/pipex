#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>

#define ERROR_ARGUMENT "Bad number of argument\n"
#define FAILED_OPEN "Open failed\n"

typedef struct	cmd
{
    char		**cmd;
    int			pipefd[2];
    struct cmd 	*next;
	struct cmd 	*prev;
    
}				cmd;

size_t		ft_strlen(const char *txt);
void    	ft_putstr(const char *txt);
char		**ft_split(char const *s, char c);
void		free_split(char **txt);

#endif