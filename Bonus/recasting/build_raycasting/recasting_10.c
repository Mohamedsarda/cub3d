#include "../../../recasting.h"

void	update_player_1(t_cub *cube)
{
	static double	last_gun_change_time;
	double			current_time;

	current_time = mlx_get_time();
	if (cube->y_press && (current_time - last_gun_change_time) > 0.1)
	{
		if (cube->gun_img[cube->current_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[cube->current_gun_index]);
			cube->gun_img[cube->current_gun_index] = NULL;
		}
		cube->current_gun_index++;
		if (cube->current_gun_index >= Y_CLICK)
			cube->current_gun_index = 0;
		last_gun_change_time = current_time;
	}
	if (cube->player->jump == 1)
		cube->player->jump_var = -100;
	else if (cube->player->jump == -1)
		((1) && (cube->player->move_speed = 3, cube->player->jump_var = 100));
	else if (cube->player->jump == 0)
		((1) && (cube->player->move_speed = 3, cube->player->jump_var = 0));
	else if (cube->player->jump == 2)
		cube->player->move_speed = 2;
}

void	check_is_wall(t_cub *cube, double new_player_x, double new_player_y)
{
	if (is_it_a_wall(new_player_x, new_player_y, cube))
	{
		cube->player->player_x = new_player_x;
		cube->player->player_y = new_player_y;
	}
	else
	{
		if (is_it_a_wall(cube->player->player_x, new_player_y, cube))
			cube->player->player_y = new_player_y;
		if (is_it_a_wall(new_player_x, cube->player->player_y, cube))
			cube->player->player_x = new_player_x;
	}
}

void	while_check_move_speed(t_cub *cube, double move_speed)
{
	double	movestep;
	double	new_player_x;
	double	new_player_y;

	while (move_speed--)
	{
		movestep = (double)cube->player->walk_direction * move_speed;
		new_player_x = cube->player->player_x
			+ (movestep) * cos(cube->player->rotat_angle);
		new_player_y = cube->player->player_y
			+ (movestep) * sin(cube->player->rotat_angle);
		if (cube->player->strafe_direction != 0)
		{
			new_player_x += (double)cube->player->strafe_direction
				* (move_speed / 1.5)
				* cos(cube->player->rotat_angle + M_PI / 2);
			new_player_y += (double)cube->player->strafe_direction
				* (move_speed / 1.5)
				* sin(cube->player->rotat_angle + M_PI / 2);
		}
		check_is_wall(cube, new_player_x, new_player_y);
	}
}

void	update_player(t_cub *cube)
{
	int		move_speed;
	t_doors	*tmp;
	int		x;
	int		y;

	update_player_1(cube);
	move_speed = cube->player->move_speed;
	cube->player->rotat_angle = normalize_angle(cube->player->rotat_angle);
	cube->player->rotat_angle += (double)cube->player->turn_direction
		* cube->player->rotation_speed;
	while_check_move_speed(cube, move_speed);
	tmp = cube->doors_locations;
	x = floor(cube->player->player_x / tile_size);
	y = floor(cube->player->player_y / tile_size);
	while (tmp)
	{
		tmp->distance = calculate_distance(x, y, tmp->x, tmp->y);
		tmp = tmp->next;
	}
}
