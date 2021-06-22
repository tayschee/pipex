#include "pipex.h"

static int		check(char t, char c)
{
	if (t == c)
		return (1);
	return (0);
}

static int		nbtab(char const *s, char c)
{
	int i;
	int count;

	i = 0;
	count = 0;
	if (*s == 0)
		return (0);
	while (s[i])
	{
		if ((check(s[i + 1], c) && check(s[i], c) == 0)
		|| (check(s[i], c) == 0 && s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char		**letsfree(char **tab, int i)
{
	int j;

	j = 0;
	while (j < i)
	{
		free(tab[j]);
		j++;
	}
	free(tab);
	return (tab);
}

static char		*creatab(char const *s, char c)
{
	char	*tabcre;
	int		i;
	int		size;

	i = 0;
	size = 0;
	while (check(s[size], c) == 0 && s[size])
		size++;
	if (!(tabcre = malloc((size + 1) * sizeof(char))))
		return (NULL);
	tabcre[size] = 0;
	while (i < size)
	{
		tabcre[i] = s[i];
		i++;
	}
	return (tabcre);
}

char			**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		ntab;
	char	**tfin;

	i = -1;
	j = 0;
	if (!s)
		return (NULL);
	ntab = nbtab(s, c);
	if (!(tfin = malloc(sizeof(char *) * (ntab + 1))))
		return (NULL);
	while (++i < ntab)
	{
		while (check(s[j], c))
			j++;
		tfin[i] = creatab(&s[j], c);
		if (tfin[i] == NULL)
			return (letsfree(tfin, i));
		while (check(s[j], c) == 0 && s[j])
			j++;
	}
	tfin[i] = 0;
	return (tfin);
}