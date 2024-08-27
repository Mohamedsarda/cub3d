#include "../../parsing.h"

int	ft_get_color_data(t_init *init, t_data *data, int type)
{
	char	**colors;
	int		i;
	int		j;
	t_color	*tmp;


	colors = ft_split(init->colors[type], ' ', '\t');
	if (!colors)
		return (-1);
	i = 0;
	while (colors[i])
		i++;
	if (i != 2)
		return (free_double_arr(colors), -1);
	if (ft_strlen(colors[0]) != 1)
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	tmp = &data->floor;
	if (type == 1)
		tmp = &data->sky;
	if (ft_count_cammas(colors[1]) != 2
		|| colors[1][ft_strlen(colors[1]) - 1] == ',')
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	ft_count_num(&i, &j, colors);
	if (i == j)
	{
		if (ft_fromdst_to_tmp(colors, tmp) == -1)
			return (free_double_arr(colors), -1);
	}
	else
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	free_double_arr(colors);
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
