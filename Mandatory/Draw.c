#include "../recasting.h"

void	draw_lines(t_cub *cube, int is)
{
	int		i;
	double	angle;

	cube->is = MAP_SCALE;
	i = 0;
	angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
	while (i < NUM_RAYS)
	{
		draw_line(cube, angle, is);
		angle -= FOV_ANGLE / NUM_RAYS;
		i++;
	}
}

void	draw_cube(t_cub *cube, int x, int y, int color)
{
	int	i;
	int	j;

	j = -1;
	while (++j < tile_size)
	{
		i = -1;
		while (++i < tile_size)
			mlx_put_pixel(cube->image, (x * tile_size) + i,
				(y * tile_size) + j, color);
	}
	return ;
}

void	draw_all_black(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			mlx_put_pixel(cube->image, x, y, c_rgba(0, 0, 0, 150));
	}
}

void	loop_fun(void *param)
{
	t_cub	*cube;
	int32_t	xpos;
	int32_t	ypos;

	cube = (t_cub *)param;
	update_player(cube);
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	draw_lines_3d(cube);
	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && cube->player->start == 0)
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start = 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
