#include "../recasting.h"

static void	ft_init_player_var(t_cub *cube, t_player *player)
{
	player->rotation_speed = 0.04;
	player->turn_direction = 0;
	player->strafe_direction = 0;
	player->walk_direction = 0;
	player->player_z = 0;
	player->start = 0;
	player->jump = 0;
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

void	ft_get_player_pos(t_player *player, t_cub *cube)
{
	int		x;
	int		y;
	t_doors	*head;
	t_doors	*new;

	y = -1;
	head = NULL;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
		{
			if (cube->data->map[y][x] == 'D' || cube->data->map[y][x] == 'O')
			{
				new = ft_lstnew_doors(x, y);
				ft_lstaddback_doors(&head, new);
			}
			if (cube->data->map[y][x] == cube->data->p)
			{
				player->player_x = (x * TILE_SIZE) + (TILE_SIZE / 2);
				player->player_y = (y * TILE_SIZE) + (TILE_SIZE / 2);
			}
		}
	}
	cube->doors_locations = head;
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
	ft_init_player_var(cube, player);
	return (player);
}
