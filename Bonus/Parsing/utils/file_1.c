#include "../../../parsing.h"

static int	ft_sign_1(char str)
{
	if (str == '-')
		return (-1);
	return (1);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(char *str)
{
	long int	nbr;
	long int	nb;
	int			sign;

	nbr = 0;
	sign = 1;
	if (!str)
		return (-1);
	if (!ft_isdigit(*str))
		return (-1);
	while ((*str == ' ' || *str == '\t' || *str == '\n'
			|| *str == '\v' || *str == '\f' || *str == '\r') && (str++));
	if (*str == '-' || *str == '+')
	{
		sign = ft_sign_1(*str);
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nb = nbr * 10 + (*str - '0');
		if (nbr > nb && sign == 1)
			return (-1);
		else if (nbr > nb && sign == -1)
			return (0);
		str++;
		nbr = nb;
	}
	if ((nbr * sign) < 0 || (nbr * sign) > 255)
		return (-1);
	return (nbr * sign);
}

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

void    ft_putstr_fd(char *str, int fd)
{
	int i;

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
