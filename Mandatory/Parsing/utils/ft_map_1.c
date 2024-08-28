#include "../../../parsing.h"

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
