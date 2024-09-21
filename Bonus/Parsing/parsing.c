/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:04 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:03:05 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../parsing.h"

static int	validate_color_input(char **colors, t_data *data)
{
	int	len;

	len = ft_strlen(colors[0]);
	if (len != 1 || ft_count_cammas(colors[1]) != 2
		|| colors[1][ft_strlen(colors[1]) - 1] == ',')
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	return (0);
}

static int	process_color_data(char **colors, t_color *tmp, t_data *data)
{
	int	i;
	int	j;

	ft_count_num(&i, &j, colors);
	if (i == j)
	{
		if (ft_fromdst_to_tmp(colors, tmp) == -1)
			return (free_double_arr(colors), -1);
	}
	else
		return (ft_check_color_print(data), -1);
	return (0);
}

int	ft_get_color_data(t_init *init, t_data *data, int type)
{
	char	**colors;
	t_color	*tmp;

	colors = ft_split(init->colors[type], ' ', '\t');
	if (!colors || !colors[0] || validate_color_input(colors, data) == -1)
		return (-1);
	tmp = &data->floor;
	if (type == 1)
		tmp = &data->sky;
	if (process_color_data(colors, tmp, data) == -1)
		return (-1);
	free_double_arr(colors);
	return (1);
}

int	ft_count_map_door(char **map)
{
	int	i;
	int	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (ft_check_char(map[i][j], 4))
			{
				if (!((map[i + 1][j] == '1' && map[i - 1][j] == '1')
					|| (map[i][j + 1] == '1' && map[i][j - 1] == '1')))
					return (-1);
			}
			j++;
		}
		i++;
	}
	return (1);
}

int	ft_get_player(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (ft_check_char(data->map[i][j], 2))
				return (data->p = data->map[i][j], 1);
			j++;
		}
		i++;
	}
	return (0);
}
