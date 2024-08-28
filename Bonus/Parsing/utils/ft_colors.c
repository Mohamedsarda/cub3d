#include "../../../parsing.h"

int	ft_count_cammas(char *ptr)
{
	int	i;
	int	comma;

	i = 0;
	comma = 0;
	if (!ptr)
		return (-1);
	while (ptr[i])
	{
		if (ptr[i] == ',' && (ptr[i + 1]
				&& ptr[i + 1] >= '0' && ptr[i + 1] <= '9'))
			comma++;
		i++;
	}
	return (comma);
}

int	ft_fromdst_to_tmp(char **colors, t_color *tmp)
{
	char	**rgb;

	rgb = ft_split(colors[1], ',', 0);
	if (!rgb)
		return (-1);
	tmp->r = ft_atoi(rgb[0]);
	tmp->g = ft_atoi(rgb[1]);
	tmp->b = ft_atoi(rgb[2]);
	free_double_arr(rgb);
	return (1);
}

void	ft_count_num(int *i, int *j, char **colors)
{
	(*j) = 0;
	(*i) = 0;
	while (colors[1][(*j)])
	{
		if ((colors[1][(*j)] >= '0'
			&& colors[1][(*j)] <= '9') || colors[1][(*j)] == ',')
			(*i)++;
		(*j)++;
	}
}

void	ft_check_color(t_data *data)
{
	if ((data->floor.r < 0 || data->floor.r > 255)
		|| (data->floor.g < 0 || data->floor.g > 255)
		|| (data->floor.b < 0 || data->floor.b > 255)
		|| (data->sky.r < 0 || data->sky.r > 255)
		|| (data->sky.g < 0 || data->sky.g > 255)
		|| (data->sky.b < 0 || data->sky.b > 255)
	)
		ft_check_color_print(data);
}

int	ft_check_char(char c, int is)
{
	if (is == 1)
		return (c == '1' || c == '0' || c == 'N' || c == 'E'
			|| c == 'W' || c == 'S' || c == 'D' || c == '$' || c == '\t');
	if (is == 2)
		return (c == 'N' || c == 'E' || c == 'W' || c == 'S');
	if (is == 3)
		return (c == '1' || c == '0' || c == 'N'
			|| c == 'E' || c == 'W' || c == 'S' || c == 'D');
	if (is == 4)
		return (c == 'D');
	return (c == '0' || c == 'N' || c == 'E'
		|| c == 'W' || c == 'S' || c == 'D');
}

