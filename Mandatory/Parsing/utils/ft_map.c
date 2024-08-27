#include "../../../parsing.h"

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

int	ft_check_map(t_data *data)
{
	int	i;
	int	j;
	int	lines;

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
				return (-1);
			if (ft_check_char(data->map[i][j], 0)
						&& (data->map[i][j + 1] == '$'
					|| (j != 0 && data->map[i][j - 1] == '$')
					|| j == 0 || j == (ft_strlen(data->map[i]) - 1)))
				return (-1);
			if (ft_check_char(data->map[i][j], 0) && ((i == (lines -1))
				|| data->map[i + 1][j] == '$'
				|| (i != 0 && data->map[i - 1][j] == '$') || i == 0))
				return (-1);
			j++;
		}
		i++;
	}
	return (1);
}
