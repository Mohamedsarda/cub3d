#include "../../parsing.h"

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

int	ft_get_color_data(t_init *init, t_data *data, int type)
{
	char	**colors;
	char	**rgb;
	int		i;
	int		j;
	t_color	*tmp;


	colors = ft_split(init->colors[type], ' ', '\t');
	i = 0;
	while (colors[i])
		i++;
	if (i != 2)
		return (free_double_arr(colors), -1);
	if (!colors)
		return (-1);
	if (ft_strlen(colors[0]) != 1)
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	tmp = &data->floor;
	if (type == 1)
		tmp = &data->sky;
	if (ft_count_cammas(colors[1]) != 2
		|| colors[1][ft_strlen(colors[1]) - 1] == ',')
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	j = 0;
	i = 0;
	while (colors[1][j])
	{
		if ((colors[1][j] >= '0' && colors[1][j] <= '9') || colors[1][j] == ',')
			i++;
		j++;
	}
	if (i == j)
	{
		rgb = ft_split(colors[1], ',', 0);
		if (!rgb)
			return (-1);
		tmp->r = ft_atoi(rgb[0]);
		tmp->g = ft_atoi(rgb[1]);
		tmp->b = ft_atoi(rgb[2]);
		free_double_arr(rgb);
	}
	else
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	free_double_arr(colors);
	return (1);
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

int	my_ft_strlen(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\t')
			j += 4;
		else
			j++;
		i++;
	}
	return (j);
}

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

int	ft_check_map(t_data *data)
{
	int	i;
	int	j;
	int	lines;

	i = 0;
	lines = 0;
	while (data->map[lines])
		lines++;
	data->map_row = lines;
	i = 0;
	if (ft_map_check_words(data->map) == -1)
		return (-1);
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (data->map[i][j] == '\t')
				return (puts("tab"), -1);
			if (ft_check_char(data->map[i][j], 0)
				&& (data->map[i][j + 1] == '$'
				|| (j != 0 && data->map[i][j - 1] == '$')
				|| j == 0 || j == (ft_strlen(data->map[i]) - 1)))
				return (-1);
			if (ft_check_char(data->map[i][j], 0)
				&& ((i == (lines -1)) || data->map[i + 1][j] == '$'
				|| (i != 0 && data->map[i - 1][j] == '$') || i == 0))
				return (-1);
			j++;
		}
		i++;
	}
	return (1);
}

void	ft_change_map(t_init *init, t_data *data)
{
	int	i = 0;
	int	j = 0;
	int len = 0;
	int tmp_len = 0;

	data->map = (char **)ft_calloc((init->file_lines + 1) * sizeof(char *));
	if (!data->map)
		return ;
	while (init->map[i])
	{
		tmp_len = ft_strlen(init->map[i]);
		if (len < tmp_len)
			len = tmp_len;
		i++;
	}
	data->map_cols = len;
	i = 0;
	while (init->map[i])
	{
		j = 0;
		data->map[i] = ft_calloc(len + 1);
		if (!data->map[i])
			return ;
		while (init->map[i][j])
		{
			if (init->map[i][j] == ' ')
				data->map[i][j] = '$';
			else
				data->map[i][j] = init->map[i][j];
			j++;
		}
		while (j < len)
			data->map[i][j++] = '$';
		data->map[i][j] = 0;
		i++;
	}
	data->map[i] = NULL;
	return ;
}

void ft_open_files(t_data *data)
{
	if (open(data->ea, O_RDONLY) < 0
		||open(data->so, O_RDONLY) < 0
		|| open(data->we, O_RDONLY) < 0
		|| open(data->no, O_RDONLY) < 0 )
	{
		ft_putstr_fd("Error: Please Check The Paths ", 2);
		ft_putstr_fd("Provided For The Textures\n", 2);
		exit(1);
	}
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
