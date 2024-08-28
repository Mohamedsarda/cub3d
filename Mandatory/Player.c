#include "../recasting.h"

void	ft_get_player_pos(t_player *player, t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
		{
			if (cube->data->map[y][x] == cube->data->p)
			{
				player->player_x = (x * tile_size) + (tile_size / 2);
				player->player_y = (y * tile_size) + (tile_size / 2);
			}
		}
	}
}

t_player	*init_player(t_cub *cube)
{
	t_player	*player;

	player = (t_player *)ft_calloc(sizeof(t_player));
	if (!player)
		malloc_error();
	ft_get_player_pos(player, cube);
	player->radius = 10;
	player->move_speed = 4;
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
	player->tab = 0;
	player->jump_var = 0;
	player->middle = 0;
	player->mouse = 0;
	player->stop_mouse = 0;
	player->right_left = 0;
	return (player);
}