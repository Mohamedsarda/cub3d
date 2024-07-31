#include "../../../parsing.h"

int ft_count_arrgs_in_file(t_init *init, char *key)
{
	int i;
	int j;
	int count;

	i = 0;
	count = 0;
	while (init->file[i])
	{
		j = 0;
		while (init->file[i][j] && (init->file[i][j] == ' ' || init->file[i][j] == '\t'))
			j++;
		if (ft_strncmp(init->file[i] + j, key, ft_strlen(key)) == 0)
			count++;
		i++;
	}
	return (count);
}

int	ft_check_2_cords_init(char **cords, t_data *data)
{
    int i = 0;
    int j = 0;
	char	**tmp;
	char	*cur;

	while (cords[i])
	{
		tmp = ft_split(cords[i], ' ', '\t');
		if (!tmp)
			return (-1);
		j = 0;
		while (tmp[j])
			j++;
		if (j != 2)
			return (free_double_arr(tmp), -1);
		cur = ft_strdup(tmp[1]);
		if (i == 0)
			data->no = cur;
		else if (i == 1)
			data->so = cur;
		else if (i == 2)
			data->we = cur;
		else if (i == 3)
			data->ea = cur;
		else
			free(cur);
		free_double_arr(tmp);
		i++;
	}
    return (1);
}
