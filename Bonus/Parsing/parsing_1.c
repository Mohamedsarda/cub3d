/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:00 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/21 23:04:39 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../parsing.h"

int	ft_map_check_words(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (!ft_check_char(map[i][j], 1))
				return (-1);
			j++;
		}
		i++;
	}
	if (ft_count_map_words(map) != 1)
		return (-1);
	if (ft_count_map_door(map) == -1)
		return (-1);
	return (1);
}

int	check_only_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == '\t' || str[i] == '$'))
		i++;
	if (str[i] != '\0')
		return (0);
	return (1);
}

int	ft_get_data(t_init *init, t_data *data)
{
	ft_open_files(data);
	if (ft_get_color_data(init, data, 0) == -1
		|| ft_get_color_data(init, data, 1) == -1)
		return (-1);
	free_double_arr(init->colors);
	ft_check_color(data);
	ft_change_map(init, data);
	if (ft_check_map(data) == -1)
		return (-1);
	ft_get_player(data);
	free_double_arr(init->file);
	free_double_arr(init->map);
	return (1);
}

int	ft_count_map_words(char **map)
{
	int	i;
	int	j;
	int	counter;

	i = 0;
	counter = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (ft_check_char(map[i][j], 2))
				counter++;
			j++;
		}
		i++;
	}
	return (counter);
}

void	ft_free_init_b(t_init *init, t_data *data)
{
	free_double_arr(init->file);
	free_double_arr(init->colors);
	free_double_arr(init->map);
	free_double_arr(init->coordinats);
	free(init);
	free(data->ea);
	free(data->we);
	free(data->so);
	free(data->no);
	free(data);
}
