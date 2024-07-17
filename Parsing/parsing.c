#include "parsing.h"

void    ff()
{
	system("leaks app");
}

int ft_count_cammas(char *ptr)
{
	int	i;
	int	comma;

	i = 0;
	comma = 0;
	if (!ptr)
		return (-1);
	while (ptr[i])
	{
		if (ptr[i] == ',' && (ptr[i + 1] && ptr[i + 1] >= '0' && ptr[i + 1] <= '9'))
			comma++;
		i++;
	}
	return (comma);
}

int ft_get_color_data(t_init *init, t_data *data, int type)
{
	char    **colors;
	char    **rgb;
	int     i;
	int     j;
	t_color	*tmp;

	colors = ft_split(init->colors[type], ' ', '\t');
	if (!colors)
		return (-1);
	free_double_arr(init->colors);
	if (ft_strlen(colors[0]) != 1)
		return (ft_check_color_print(data), -1);
	tmp = &data->floor;
	if (type == 1)
		tmp = &data->sky;
	if (ft_count_cammas(colors[1]) != 2)
		return (ft_check_color_print(data), -1);
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
	}
	else
		return (free_double_arr(colors), ft_check_color_print(data), -1);
	free_double_arr(colors);
	return (1);
}

void    my_print_color(t_data *data)
{
	printf("R : [%d]", data->floor.r);
	printf("G : [%d]", data->floor.g);
	printf("B : [%d]\n", data->floor.b);
	printf("R : [%d]", data->sky.r);
	printf("G : [%d]", data->sky.g);
	printf("B : [%d]\n", data->sky.b);
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

int ft_get_data(t_init *init, t_data *data)
{
	data = (t_data *)ft_calloc(sizeof(t_data));
	if (!data)
		return (-1);
	ft_get_color_data(init, data, 0);
	ft_get_color_data(init, data, 1);
	free_double_arr(init->colors);
	ft_check_color(data);
	// ft_get_coordinats_data(init, data);
	my_print_color(data);
	return (1);
}

void    my_print(t_init *init)
{
	int i = 0;
	while (init->map && init->map[i])
	{
		printf("[%s]\n", init->map[i++]);
	}
	// i = 0;
	// puts("\n|\n");
	// while (init->colors && init->colors[i])
	// {
	// 	printf("%s", init->colors[i++]);
	// }
	// i = 0;
	// puts("|\n");
	// while (init->coordinats && init->coordinats[i])
	// {
	// 	printf("%s", init->coordinats[i++]);
	// }
}


int main()
{
	atexit(ff);
	int fd = open("./Maps/map_1.cub", O_RDONLY);
	char *tmp;

	if (fd == -1 || fd == -1)
		return (1);
	t_init *init;
	t_data data;

	init = (t_init *)ft_calloc(sizeof(t_init));
	if (!init)
		return (1);
	int i = 0;
	while (1)
	{
		tmp = get_next_line(fd);
		if (!tmp)
			break ;
		free(tmp);
		i++;
	}
	init->file_lines = i;
	close(fd);
	// printf("%d | \n", i);
	fd = open("./Maps/map_1.cub", O_RDONLY);
	init->file = (char **)ft_calloc((i + 1) * sizeof(t_init *));
	i = 0;
	while (1)
	{
		init->file[i] = get_next_line(fd);
		if (!init->file[i])
			break ;
		i++;
	}
	close(fd);
	if (ft_get_data_init(init, &data) == -1)
		return (1);
	printf("[%s]\n", data.no);
	printf("[%s]\n", data.so);
	printf("[%s]\n", data.we);
	printf("[%s]\n", data.ea);
	ft_get_data(init, &data);
	// my_print(init);
	//
	
	//clean
	// free_double_arr(init->file);
}
