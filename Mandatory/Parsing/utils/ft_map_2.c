/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:57 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:03:58 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../parsing.h"

int	ft_check_edges(char **map, int i, int j, int lines)
{
	int	len;

	len = ft_strlen(map[i]);
	if (ft_check_char(map[i][j], 0)
		&& (map[i][j + 1] == '$' || j == 0 || j == len - 1
		|| (j != 0 && map[i][j - 1] == '$')))
		return (-1);
	if (ft_check_char(map[i][j], 0)
		&& (i == lines - 1 || map[i + 1][j] == '$'
		|| (i != 0 && map[i - 1][j] == '$') || i == 0))
		return (-1);
	return (0);
}

int	ft_check_map_row(char **map, int i, int lines)
{
	int	j;

	j = 0;
	while (map[i][j])
	{
		if (map[i][j] == '\t' || ft_check_edges(map, i, j, lines) == -1)
			return (-1);
		j++;
	}
	return (0);
}

void	ft_open_files(t_data *data)
{
	if (open(data->ea, O_RDONLY) < 0
		||open(data->so, O_RDONLY) < 0
		|| open(data->we, O_RDONLY) < 0
		|| open(data->no, O_RDONLY) < 0)
	{
		ft_putstr_fd("Error: Please Check ", 2);
		ft_putstr_fd("The Paths Provided For The Textures\n", 2);
		exit(1);
	}
}

int	ft_check_map(t_data *data)
{
	int	i;
	int	lines;

	lines = 0;
	while (data->map[lines])
		lines++;
	data->map_row = lines;
	if (ft_map_check_words(data->map) == -1)
		return (-1);
	i = 0;
	while (data->map[i])
	{
		if (ft_check_map_row(data->map, i, lines) == -1)
			return (-1);
		i++;
	}
	return (1);
}
