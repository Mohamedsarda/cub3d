#include "../recasting.h"
#include "../parsing.h"

void	ft_check_line(char *tmp, int i)
{
	if (ft_strncmp(tmp, "1", 1)
		&& ft_strncmp(tmp, "C ", 2) != 0
		&& ft_strncmp(tmp, "F ", 2) != 0
		&& ft_strncmp(tmp, "EA ", 2) != 0
		&& ft_strncmp(tmp, "WE ", 2) != 0
		&& ft_strncmp(tmp, "SO ", 2) != 0
		&& ft_strncmp(tmp, "NO ", 2) != 0)
	{
		printf("Please Remove This unnecessary");
		printf(" Line From The File, It's on this line %d\n", i + 1);
		exit (1);
	}
}

void	ft_check_extention(char *dst)
{
	int	i;

	i = 0;
	while (*dst)
	{
		if (*dst == '.' && i > 2)
			break ;
		dst++;
		i++;
	}
	if (!dst[0] || ft_strncmp(dst, ".cub", ft_strlen(dst)) != 0)
	{
		printf("The Map Should End With .cub\n");
		exit (1);
	}
}

void	ft_count_file_lines(t_init *init, char **dst)
{
	char	*tmp;
	int		fd;
	int		i;

	ft_check_extention(dst[1]);
	fd = open(dst[1], O_RDONLY);
	if (fd <= 0)
	{
		ft_putstr_fd("The Path Provided to the map is wrong\n", 2);
		exit(1);
	}
	i = 0;
	while (1)
	{
		tmp = get_next_line(fd);
		if (tmp && (ft_strncmp(tmp, " ", ft_strlen(tmp)) != 0
				|| ft_strncmp(tmp, "\t", ft_strlen(tmp)) != 0))
			ft_check_line(tmp, i);
		if (!tmp)
			break ;
		free(tmp);
		i++;
	}
	init->file_lines = i;
	close(fd);
}

void	ft_read_file_0(t_init *init, char **dst)
{
	int	i;
	int	fd;

	i = 0;
	fd = open(dst[1], O_RDONLY);
	if (fd <= 0)
	{
		ft_putstr_fd("The Path Provided to the map is wrong\n", 2);
		exit(1);
	}
	while (1)
	{
		init->file[i] = get_next_line(fd);
		if (!init->file[i])
			break ;
		i++;
	}
	close(fd);
}

void	ft_cube_func(t_cub *cube)
{
	cube->data->height = cube->data->map_row * tile_size;
	cube->data->width = cube->data->map_cols * tile_size;
	ft_fractol_init(cube);
	mlx_key_hook(cube->mlx, &my_keyhook, cube);
	mlx_loop_hook(cube->mlx, loop_fun, cube);
	mlx_close_hook(cube->mlx, &close_window, cube);
	mlx_loop(cube->mlx);
}

void	f()
{
	system("leaks cub3D");
}

int	main(int c, char **dst)
{
	t_init	*init;
	t_data	*data;
	t_cub	cube;
	atexit(f);
	if (c != 2)
	{
		ft_putstr_fd("To Play the game u need to provide a map\n", 2);
		return (1);
	}
	data = NULL;
	data = (t_data *)ft_calloc(sizeof(t_data));
	init = (t_init *)ft_calloc(sizeof(t_init));
	if (!init)
		return (1);
	ft_count_file_lines(init, dst);
	init->file = (char **)ft_calloc((init->file_lines + 1) * sizeof(t_init *));
	ft_read_file_0(init, dst);
	if (ft_get_data_init(init, data) == -1)
		return (1);
	if (ft_get_data(init, data) == -1)
		return (ft_check_map_print(data), 1);
	free(init);
	cube.data = data;
	ft_cube_func(&cube);
}
