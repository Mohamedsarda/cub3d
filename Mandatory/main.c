/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:04:15 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:04:16 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../recasting.h"
#include "../parsing.h"

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
	cube->data->height = cube->data->map_row * TILE_SIZE;
	cube->data->width = cube->data->map_cols * TILE_SIZE;
	ft_fractol_init(cube);
	mlx_key_hook(cube->mlx, &my_keyhook, cube);
	mlx_loop_hook(cube->mlx, loop_fun, cube);
	mlx_close_hook(cube->mlx, &close_window, cube);
	mlx_loop(cube->mlx);
}

void f()
{
	system("leaks cub3D");
}

void	ft_free_init(t_init *init, t_data *data)
{
	free_double_arr(init->file);
	free_double_arr(init->colors);
	free_double_arr(init->map);
	free_double_arr(init->coordinats);
	free(init);
	free(data->ea);
	free(data->we);
	free(data->so);
	free(data->no);
	free(data);
}

int	main(int c, char **dst)
{
	t_init	*init;
	t_data	*data;
	t_cub	cube;

	if (c != 2)
	{
		ft_putstr_fd("To Play the game u need to provide a map\n", 2);
		return (1);
	}
	atexit(f);
	data = NULL;
	data = (t_data *)ft_calloc(sizeof(t_data));
	init = (t_init *)ft_calloc(sizeof(t_init));
	if (!init)
		return (1);
	ft_count_file_lines(init, dst);
	init->file = (char **)ft_calloc((init->file_lines + 1) * sizeof(t_init *));
	ft_read_file_0(init, dst);
	if (ft_get_data_init(init, data) == -1)
		return (ft_free_init(init, data), 1);
	if (ft_get_data(init, data) == -1)
		return (ft_check_map_print(data), 1);
	free(init);
	cube.data = data;
	ft_cube_func(&cube);
}
