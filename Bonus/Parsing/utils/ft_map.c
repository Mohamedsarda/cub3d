/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:02:54 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:02:55 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../parsing.h"

static int	get_max_len(char **map)
{
	int	len;
	int	tmp_len;
	int	i;

	len = 0;
	i = 0;
	while (map[i])
	{
		tmp_len = ft_strlen(map[i]);
		if (len < tmp_len)
			len = tmp_len;
		i++;
	}
	return (len);
}

static void	fill_map_row(char *src, char *dst, int len)
{
	int	j;

	j = 0;
	while (src[j])
	{
		if (src[j] == ' ')
			dst[j] = '$';
		else
			dst[j] = src[j];
		j++;
	}
	while (j < len)
		dst[j++] = '$';
	dst[j] = 0;
}

void	ft_change_map(t_init *init, t_data *data)
{
	int	i;
	int	len;

	len = get_max_len(init->map);
	data->map = (char **)ft_calloc((init->file_lines + 1) * sizeof(char *));
	if (!data->map)
		return ;
	data->map_cols = len;
	i = 0;
	while (init->map[i])
	{
		data->map[i] = ft_calloc(len + 1);
		if (!data->map[i])
			return ;
		fill_map_row(init->map[i], data->map[i], len);
		i++;
	}
	data->map[i] = NULL;
}
