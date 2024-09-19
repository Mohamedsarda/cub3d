#include "../../../recasting.h"

void	init_player_1(t_cub *cube, t_player *player)
{
	player->minimap = 1;
	player->tab = 0;
	player->jump_var = 0;
	player->middle = 0;
	player->mouse = 0;
	player->stop_mouse = 0;
	player->right_left = 0;
	cube->current_gun_index = 0;
	cube->cur_g_right_clikc = 100;
	cube->y_press = 0;
	cube->t_press = 0;
	player->shift = 1;
	player->open = 1;
	cube->right_press = 0;
}

t_player	*init_player(t_cub *cube)
{
	t_player	*player;

	player = (t_player *)ft_calloc(sizeof(t_player));
	if (!player)
		malloc_error();
	ft_get_player_pos(player, cube);
	player->radius = 10;
	player->move_speed = 0;
	if (cube->data->p == 'W')
		player->rotat_angle = deg2rad(180);
	else if (cube->data->p == 'S')
		player->rotat_angle = deg2rad(90);
	else if (cube->data->p == 'N')
		player->rotat_angle = deg2rad(270);
	else if (cube->data->p == 'E')
		player->rotat_angle = deg2rad(0);
	player->rotation_speed = 0.04;
	player->turn_direction = 0;
	player->strafe_direction = 0;
	player->walk_direction = 0;
	player->player_z = 0;
	player->start = 0;
	player->jump = 0;
	init_player_1(cube, player);
	return (player);
}

int	c_rgba(int r, int g, int b, int a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

void	draw_cube(t_cub *cube, int x, int y, int color)
{
	int	i;
	int	j;

	j = -1;
	while (++j < TILE_SIZE)
	{
		i = -1;
		while (++i < TILE_SIZE)
			mlx_put_pixel(cube->image, (x * TILE_SIZE) + i,
				(y * TILE_SIZE) + j, color);
	}
	return ;
}

double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}
