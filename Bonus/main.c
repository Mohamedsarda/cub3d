/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:38 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/22 00:06:57 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../recasting.h"
#include "../parsing.h"

void	ft_count_file_lines_while(int fd, int *i)
{
	char	*tmp;

	while (1)
	{
		tmp = get_next_line(fd);
		if (!tmp)
			break ;
		free(tmp);
		(*i)++;
	}
}

void	ft_count_file_lines(t_init *init, char **dst)
{
	int		fd;
	int		i;

	i = 0;
	ft_check_extention(dst[1]);
	fd = open(dst[1], O_RDONLY);
	if (fd <= 0)
	{
		ft_putstr_fd("The Path Provided to the map is wrong\n", 2);
		exit(1);
	}
	i = 0;
	ft_count_file_lines_while(fd, &i);
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
	mlx_mouse_hook(cube->mlx, &my_mousehook, cube);
	mlx_loop_hook(cube->mlx, loop_fun, cube);
	mlx_close_hook(cube->mlx, &close_window, cube);
	mlx_loop(cube->mlx);
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
	data = NULL;
	data = (t_data *)ft_calloc(sizeof(t_data));
	init = (t_init *)ft_calloc(sizeof(t_init));
	if (!init)
		return (1);
	ft_count_file_lines(init, dst);
	init->file = (char **)ft_calloc((init->file_lines + 1) * sizeof(t_init *));
	ft_read_file_0(init, dst);
	if (ft_get_data_init(init, data) == -1)
		return (ft_free_init_b(init, data), 1);
	if (ft_get_data(init, data) == -1)
		return (ft_check_map_print(data), 1);
	free(init);
	cube.data = data;
	ft_cube_func(&cube);
}
