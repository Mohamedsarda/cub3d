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
