#include "../../../parsing.h"

static int	ft_get_start_pos(t_init *init, char *c)
{
	int	i;
	int	j;

	i = 0;
	while (init->file[i])
	{
		j = 0;
		while (init->file[i][j] && (init->file[i][j] == ' '
				|| init->file[i][j] == '\t'))
			j++;
		if (ft_strncmp(init->file[i] + j, c, ft_strlen(c)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_get_data_init(t_init *init, t_data *data)
{
	int	i;
	int	j;
	int	map;

	ft_check_file(init);
	i = ft_get_start_pos(init, "1");
	j = ft_get_start_pos(init, "0");
	if ((j != -1) && j < i)
		i = j;
	if (i == -1)
		return (-1);
	j = 0;
	map = init->file_lines - i;
	while (ft_strncmp(init->file[--init->file_lines], "\n", 1) == -10)
		map--;
	init->map = (char **)ft_calloc((map + 1) * sizeof(char *));
	if (!init->map)
		return (-1);
	while (j < map)
		init->map[j++] = ft_strdup(init->file[i++]);
	init->map[j] = NULL;
	ft_get_colors_init(init);
	if (ft_get_coordinats_init(init, data) == -1)
		ft_print_cords_err(init);
	return (1);
}

int	ft_get_coordinats_init(t_init *init, t_data *data)
{
	int	no;
	int	so;
	int	we;
	int	ea;

	no = ft_get_start_pos(init, "NO");
	so = ft_get_start_pos(init, "SO");
	we = ft_get_start_pos(init, "WE");
	ea = ft_get_start_pos(init, "EA");
	if (no == -1 || so == -1 || we == -1 || ea == -1)
		return (-1);
	init->coordinats = (char **)ft_calloc(5 * sizeof(char *));
	if (!init->coordinats)
		return (-1);
	init->coordinats[0] = ft_strdup(init->file[no]);
	init->coordinats[1] = ft_strdup(init->file[so]);
	init->coordinats[2] = ft_strdup(init->file[we]);
	init->coordinats[3] = ft_strdup(init->file[ea]);
	init->coordinats[4] = NULL;
	if (ft_check_2_cords_init(init->coordinats, data) == -1)
		return (free_double_arr(init->coordinats), -1);
	free_double_arr(init->coordinats);
	return (1);
}

int	ft_get_colors_init(t_init *init)
{
	int	f;
	int	c;

	f = ft_get_start_pos(init, "F");
	c = ft_get_start_pos(init, "C");
	if (c == -1 || f == -1)
		return (-1);
	init->colors = (char **)ft_calloc(3 * sizeof(char *));
	if (!init->colors)
		return (-1);
	init->colors[0] = ft_strdup(init->file[f]);
	init->colors[1] = ft_strdup(init->file[c]);
	init->colors[2] = NULL;
	return (1);
}
