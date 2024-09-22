/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:02:44 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:02:47 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int	ft_skip_whitespace(char **str)
{
	while (**str == ' ' || **str == '\t' || **str == '\n'
		|| **str == '\v' || **str == '\f' || **str == '\r')
		(*str)++;
	if (!ft_isdigit(**str) && **str != '-' && **str != '+')
		return (-1);
	return (0);
}

static int	ft_handle_overflow(long nbr, long nb, int sign)
{
	if (nbr > nb && sign == 1)
		return (-1);
	if (nbr > nb && sign == -1)
		return (0);
	return (1);
}

int	ft_atoi(char *str)
{
	long	nbr;
	int		sign;

	nbr = 0;
	sign = 1;
	if (!str || ft_skip_whitespace(&str) == -1)
		return (-1);
	sign = ft_sign_1(*str);
	while (*str >= '0' && *str <= '9')
	{
		if (ft_handle_overflow(nbr, nbr * 10 + (*str - '0'), sign) != 1)
			return (-1);
		nbr = nbr * 10 + (*str++ - '0');
	}
	if ((nbr * sign) < 0 || (nbr * sign) > 255)
		return (-1);
	return (nbr * sign);
}
