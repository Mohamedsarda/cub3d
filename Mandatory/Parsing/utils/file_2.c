#include "../../../parsing.h"

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	if (n == 0)
		return (0);
	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((i < n - 1) && (str1[i] && str2[i]) && (str1[i] == str2[i]))
		i++;
	return (str1[i] - str2[i]);
}

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
		write (fd, &str[i++], 1);
}

void	free_double_arr(char **tmp)
{
	int	i;

	i = 0;
	if (!tmp || !*tmp)
		return ;
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
}