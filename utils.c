#include "pipex.h"

size_t		ft_strlen(const char *txt)
{
    size_t i = 0;

    while (txt[i])
    {
        ++i;
    }
    return i;
}

void    	ft_putstr(const char *txt)
{
    write(1, txt, ft_strlen(txt));
}

void		free_split(char **txt)
{
	int i = 0;
	while (txt[i])
	{
		free(txt[i]);
		++i;
	}
	free(txt);
	txt = NULL;
}