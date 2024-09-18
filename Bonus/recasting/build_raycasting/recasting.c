#include "../../../recasting.h"
#include <time.h>

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

void	close_window(void *param)
{
	t_cub	*cube;

	cube = (t_cub *)param;
	ft_free_data(cube);
	mlx_delete_image(cube->mlx, cube->image);
	mlx_close_window(cube->mlx);
	exit(0);
}

void	ft_threads(t_cub *cube)
{
	ft_draw_sky_floor(cube);
	pthread_create(&cube->threads[0].thread,
		NULL, &draw_lines_3d, (void *)cube);
	pthread_create(&cube->threads[1].thread,
		NULL, &draw_lines_3d_1, (void *)cube);
	pthread_join(cube->threads[0].thread, NULL);
	pthread_join(cube->threads[1].thread, NULL);
}

void	loop_fun(void *param)
{
	t_cub	*cube;
	int32_t	xpos;
	int32_t	ypos;

	cube = (t_cub *)param;
	update_player(cube);
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	update_player(cube);
	ft_threads(cube);
	draw_per(cube);
	update_y_press(cube);
	draw_gun_right_click(cube);
	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2
		&& (cube->player->start == 0 || cube->player->start == 1))
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start += 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
