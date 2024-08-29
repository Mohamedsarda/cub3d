#include "../recasting.h"

void	ft_key_press(t_cub *cube, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_RIGHT)
		cube->player->turn_direction = 1;
	if (keydata.key == MLX_KEY_LEFT)
		cube->player->turn_direction = -1;
	if (keydata.key == MLX_KEY_W)
		cube->player->walk_direction = 1;
	if (keydata.key == MLX_KEY_UP)
		cube->player->walk_direction = 1;
	if (keydata.key == MLX_KEY_S)
		cube->player->walk_direction = -1;
	if (keydata.key == MLX_KEY_DOWN)
		cube->player->walk_direction = -1;
	if (keydata.key == MLX_KEY_D)
		cube->player->strafe_direction = 1;
	if (keydata.key == MLX_KEY_A)
		cube->player->strafe_direction = -1;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		ft_free_data(cube);
		mlx_close_window(cube->mlx);
		mlx_delete_image(cube->mlx, cube->image);
	}
}

void	my_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub	*cube;

	(void)keydata;
	(void)param;
	cube = param;
	if (keydata.action == MLX_PRESS)
		ft_key_press(cube, keydata);
	if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_DOWN)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_S)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_UP)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
			cube->player->turn_direction = 0;
		if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A)
			cube->player->strafe_direction = 0;
	}
}

void	handle_mouse(t_cub *cube)
{
	static int32_t	prev_xpos;
	int32_t			xpos;
	int32_t			ypos;
	int32_t			delta_x;

	prev_xpos = WIDTH / 2;
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	if (xpos < 0 || xpos > WIDTH || ypos < 0 || ypos > HEIGHT)
		return ;
	delta_x = xpos - prev_xpos;
	cube->player->rotat_angle += delta_x * 0.001;
	mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
	mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
}

void	ft_strafe(t_cub *cube, double *new_player_x,
		double *new_player_y, double move_speed)
{
	if (cube->player->strafe_direction != 0)
	{
		(*new_player_x) += (double)cube->player->strafe_direction
			* (move_speed / 1.5)
			* cos(cube->player->rotat_angle + M_PI / 2);
		(*new_player_y) += (double)cube->player->strafe_direction
			* (move_speed / 1.5)
			* sin(cube->player->rotat_angle + M_PI / 2);
	}
	if (is_it_a_wall((*new_player_x), (*new_player_y), cube))
	{
		cube->player->player_x = (*new_player_x);
		cube->player->player_y = (*new_player_y);
	}
	else
	{
		if (is_it_a_wall(cube->player->player_x, (*new_player_y), cube))
			cube->player->player_y = (*new_player_y);
		if (is_it_a_wall((*new_player_x), cube->player->player_y, cube))
			cube->player->player_x = (*new_player_x);
	}
}

void	update_player(t_cub *cube)
{
	int		move_speed;
	double	new_player_x;
	double	new_player_y;
	double	movestep;

	move_speed = cube->player->move_speed;
	cube->player->rotat_angle = normalize_angle(cube->player->rotat_angle);
	cube->player->rotat_angle += (double)cube->player->turn_direction
		* cube->player->rotation_speed;
	while (move_speed--)
	{
		movestep = (double)cube->player->walk_direction * move_speed;
		new_player_x = cube->player->player_x
			+ movestep * cos(cube->player->rotat_angle);
		new_player_y = cube->player->player_y
			+ movestep * sin(cube->player->rotat_angle);
		ft_strafe(cube, &new_player_x, &new_player_y, move_speed);
	}
}
