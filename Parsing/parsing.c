#include "parsing.h"

void    ff()
{
	system("leaks a.out");
}


static int	ft_sign_1(const char str)
{
	if (str == '-')
		return (-1);
	return (1);
}

int	ft_atoi(const char *str)
{
	long int	nbr;
	long int	nb;
	int			sign;

	nbr = 0;
	sign = 1;
	while ((*str == ' ' || *str == '\t' || *str == '\n'
			|| *str == '\v' || *str == '\f' || *str == '\r') && (str++));
	if (*str == '-' || *str == '+')
	{
		sign = ft_sign_1(*str);
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		nb = nbr * 10 + (*str - '0');
		if (nbr > nb && sign == 1)
			return (-1);
		else if (nbr > nb && sign == -1)
			return (0);
		str++;
		nbr = nb;
	}
	if ((nbr * sign) < 0 || (nbr * sign) > 255)
		return (-1);
	return (nbr * sign);
}
void	free_double_arr(char **tmp)
{
	int	i;

	i = 0;
	if (!tmp || !*tmp)
		return ;
	while (tmp[i])
		free(tmp[i++]);
	free(tmp);
}

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	if (n == 0)
		return (0);
	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while ((i < n - 1) && (str1[i] && str2[i]) && (str1[i] == str2[i]))
		i++;
	return (str1[i] - str2[i]);
}

int ft_count_arrgs_int_file(t_init *init, char *key)
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

int ft_get_start_pos(t_init *init, char *c)
{
	int i;
	int j;

	i = 0;
	while (init->file[i])
	{
		j = 0;
		while (init->file[i][j] && (init->file[i][j] == ' ' || init->file[i][j] == '\t'))
			j++;
		if (ft_strncmp(init->file[i] + j, c, ft_strlen(c)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void    ft_putstr_fd(char *str, int fd)
{
	int i;

	i = 0;
	while (str[i])
		write (fd, &str[i++], 1);
}

void	ft_check_color_print(t_init *init, t_data *data)
{
	ft_putstr_fd("Error: Please Check Your Map\n", 2);
	free_double_arr(init->file);
	free_double_arr(init->map);
	free_double_arr(init->colors);
	free_double_arr(init->coordinats);
	//
	free_double_arr(data->coordinats);
	free_double_arr(data->map);
	//
	exit (1);
}

void    ft_check_file(t_init *init)
{
	int i;

	i = 0;
	if (ft_count_arrgs_int_file(init, "NO") != 1
		|| ft_count_arrgs_int_file(init, "SO") != 1
		|| ft_count_arrgs_int_file(init, "WE") != 1
		|| ft_count_arrgs_int_file(init, "EA") != 1
		|| ft_count_arrgs_int_file(init, "F") != 1
		|| ft_count_arrgs_int_file(init, "C") != 1
		)
	{
		ft_putstr_fd("Error: Please Check Your Map\n", 2);
		free_double_arr(init->file);
		exit (1);
	}
}

int ft_get_coordinats_init(t_init *init)
{
	int no;
	int so;
	int we;
	int ea;

	no = ft_get_start_pos(init, "NO");
	so = ft_get_start_pos(init, "SO");
	we = ft_get_start_pos(init, "WE");
	ea = ft_get_start_pos(init, "EA");
	if (no == -1 || so == -1 || we == -1 || ea == -1)
		return (-1);
	// i need to check if if there is only one NO and one SO ... #done
	init->coordinats = (char **)ft_calloc(5 * sizeof(char *));
	if (!init->coordinats)
		return (-1);
	init->coordinats[0] = ft_strdup(init->file[no]);
	init->coordinats[1] = ft_strdup(init->file[so]);
	init->coordinats[2] = ft_strdup(init->file[we]);
	init->coordinats[3] = ft_strdup(init->file[ea]);
	init->coordinats[4] = NULL;
	return (1);
}

int ft_get_colors_init(t_init *init)
{
	int f;
	int c;

	f = ft_get_start_pos(init, "F");
	c = ft_get_start_pos(init, "C");
	if (c == -1 || f == -1)
		return (-1);
	// i need to count if F is only 1 or if C is only one ..... #done
	init->colors = (char **)ft_calloc(3 * sizeof(char *));
	if (!init->colors)
		return (-1);
	init->colors[0] = ft_strdup(init->file[f]);
	init->colors[1] = ft_strdup(init->file[c]);
	init->colors[2] = NULL;
	return (1);
}

int ft_get_data_init(t_init *init)
{
	int i;
	int j;

	ft_check_file(init);
	i = ft_get_start_pos(init, "1");
	j = ft_get_start_pos(init, "0");
	if ((j != -1) && j < i)
		i = j;
	if (i == -1)
		return (-1);
	j = 0;
	init->map = (char **)ft_calloc((init->file_lines - i + 1) * sizeof(char *));
	if (!init->map)
		return (-1);
	while (init->file[i])
		init->map[j++] = ft_strdup(init->file[i++]);
	init->map[j] = NULL;
	ft_get_colors_init(init);
	ft_get_coordinats_init(init);
	return (1);
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
	tmp = &data->floor;
	if (type == 1)
		tmp = &data->sky;
	if (ft_count_cammas(colors[1]) != 2)
		return (ft_check_color_print(init, data), -1);
	j = 0;
	i = 0;
	while (colors[1][j])
	{
		if ((colors[1][j] >= '0' && colors[1][j] <= '9') || colors[1][j] == ',')
			i++;
		j++;
	}
	if ((i + 1) == j)
	{
		rgb = ft_split(colors[1], ',', 0);
		if (!rgb)
			return (-1);
		tmp->r = ft_atoi(rgb[0]);
		tmp->g = ft_atoi(rgb[1]);
		tmp->b = ft_atoi(rgb[2]);
	}
	else
		return (ft_check_color_print(init, data), -1);
	return (1);
}

// void    my_print(t_data *data)
// {
// 	int i = 0;
// 	printf("R : [%d]", data->floor.r);
// 	printf("G : [%d]", data->floor.g);
// 	printf("B : [%d]\n", data->floor.b);
// 	printf("R : [%d]", data->sky.r);
// 	printf("G : [%d]", data->sky.g);
// 	printf("B : [%d]\n", data->sky.b);
// }

void	ft_check_color(t_init *init, t_data *data)
{
	if ((data->floor.r < 0 || data->floor.r > 255)
		|| (data->floor.g < 0 || data->floor.g > 255)
		|| (data->floor.b < 0 || data->floor.b > 255)
		|| (data->sky.r < 0 || data->sky.r > 255)
		|| (data->sky.g < 0 || data->sky.g > 255)
		|| (data->sky.b < 0 || data->sky.b > 255)
	)
		ft_check_color_print(init, data);
}

// int	ft_get_coordinats_data(t_init *init, t_data *data)
// {
	
// }

int ft_get_data(t_init *init, t_data *data)
{
	data = (t_data *)ft_calloc(sizeof(t_data));
	if (!data)
		return (-1);
	ft_get_color_data(init, data, 0);
	ft_get_color_data(init, data, 1);
	ft_check_color(init, data);
	// ft_get_coordinats_data(init, data);
	// my_print(data);
	return (1);
}

void    my_print(t_init *init)
{
	int i = 0;
	while (init->map && init->map[i])
	{
		printf("%s", init->map[i++]);
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
	// atexit(ff);
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
	if (ft_get_data_init(init) == -1)
		return (1);
	ft_get_data(init, &data);
	my_print(init);
	//
	
	//clean
	// free_double_arr(init->file);
}
