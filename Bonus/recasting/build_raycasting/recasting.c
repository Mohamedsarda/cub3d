#include "../../../recasting.h"


// Add this function to your code
void draw_textured_floor(t_cub *cube)
{
	// Player's direction
	double dir_x = cos(cube->player->rotat_angle);
	double dir_y = sin(cube->player->rotat_angle);

	double plane_x = -dir_y;
	double plane_y = dir_x;

	for (int y = HEIGHT / 2 + 1; y < HEIGHT; y++)
	{
		int p = y - (HEIGHT / 2 + cube->player->player_z + cube->player->jump_var);

		double pos_z = 0.5 * HEIGHT;

		double row_distance = pos_z / p;

		double step_x = row_distance * (dir_x + plane_x) / WIDTH;
		double step_y = row_distance * (dir_y + plane_y) / WIDTH;

		double floor_x = cube->player->player_x + row_distance * (dir_x - plane_x);
		double floor_y = cube->player->player_y + row_distance * (dir_y - plane_y);

		for(int x = 0; x < WIDTH; ++x)
		{
			int cell_x = (int)(floor_x);
			int cell_y = (int)(floor_y);

			int tx = (int)(cube->texture[0]->width * (floor_x - cell_x)) & (cube->texture[0]->width - 1);
			int ty = (int)(cube->texture[0]->height * (floor_y - cell_y)) & (cube->texture[0]->height - 1);

			floor_x += step_x;
			floor_y += step_y;

			uint32_t color = get_pixel_color(cube->texture[0], tx, ty);
			mlx_put_pixel(cube->image, x, y, color);
		}
	}
}


void	update_y_press(t_cub *cube)
{
	static int	prev_gun_index;

	prev_gun_index = -1;
	if (cube->y_press)
	{
		if (prev_gun_index != -1 && cube->gun_img[prev_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);
			cube->gun_img[prev_gun_index] = NULL;
		}
		if (!cube->gun_img[cube->current_gun_index])
		{
			cube->gun_img[cube->current_gun_index] = mlx_texture_to_image(cube->mlx, cube->gun[cube->current_gun_index]);
			if (!cube->gun_img[cube->current_gun_index])
				ft_error();
		}
		if (mlx_image_to_window(cube->mlx, cube->gun_img[cube->current_gun_index],
				WIDTH / 2 - cube->gun[cube->current_gun_index]->width / 2,
				HEIGHT - cube->gun[cube->current_gun_index]->height) < 0)
			ft_error();
		prev_gun_index = cube->current_gun_index;
		if (cube->current_gun_index == Y_CLICK - 1)
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);
			cube->y_press = 0;
		}
	}
}

void	handle_mouse(t_cub *cube)
{
	int32_t	prev_xpos;
	int32_t	xpos;
	int32_t	ypos;
	double	sensitivity;
	int32_t	delta_x;
	int32_t prev_ypos;
	int32_t delta_y;

	prev_xpos = WIDTH / 2;
	sensitivity = 0.001;
	mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	delta_x = xpos - prev_xpos;
	cube->player->rotat_angle += delta_x * sensitivity;
	mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
	prev_ypos = HEIGHT / 2;
	delta_y = ypos - prev_ypos;
	cube->player->player_z = cube->player->player_z + delta_y;
}

void	update_player(t_cub *cube)
{
	static double	last_gun_change_time;
	double			current_time;

	last_gun_change_time = 0;
	current_time = mlx_get_time();
	if (cube->y_press && (current_time - last_gun_change_time) > 0.15)
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
	{
		cube->player->move_speed = 3;
		cube->player->jump_var = 100;
	}
	else if (cube->player->jump == 0)
	{
		cube->player->move_speed = 3;
		cube->player->jump_var = 0;
	}
	else if (cube->player->jump == 2)
		cube->player->move_speed = 2;
	int move_speed = cube->player->move_speed;
	cube->player->rotat_angle = normalize_angle(cube->player->rotat_angle);
	cube->player->rotat_angle += (double)cube->player->turn_direction * cube->player->rotation_speed;
	while (move_speed--)
	{
		// Calculate movement step
		double movestep = (double)cube->player->walk_direction * move_speed;
		double new_player_x;
		double new_player_y;
		new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
		new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

		if (cube->player->strafe_direction != 0)
		{
			new_player_x += (double)cube->player->strafe_direction * (move_speed / 1.5) * cos(cube->player->rotat_angle + M_PI / 2);
			new_player_y += (double)cube->player->strafe_direction * (move_speed / 1.5) * sin(cube->player->rotat_angle + M_PI / 2);
		}

		// Check for wall collision before updating player position
		if (is_it_a_wall(new_player_x, new_player_y, cube))
		{
			// No collision, update both x and y
			cube->player->player_x = new_player_x;
			cube->player->player_y = new_player_y;
		}
		else
		{
			// Check for sliding along the wall
			if (is_it_a_wall(cube->player->player_x, new_player_y, cube))
				cube->player->player_y = new_player_y;
			if (is_it_a_wall(new_player_x, cube->player->player_y, cube))
				cube->player->player_x = new_player_x;
		}
	}
	// check if player is near a door
	t_doors *tmp = cube->doors_locations;
	int x = floor(cube->player->player_x / tile_size);
	int y = floor(cube->player->player_y / tile_size);
	while (tmp)
	{
		tmp->distance = calculate_distance(x, y, tmp->x, tmp->y);
		tmp = tmp->next;
	}
}

#include <time.h>

#define TARGET_FPS 60
#define FRAME_TARGET_TIME (1000 / TARGET_FPS)

void loop_fun(void* param)
{
	t_cub* cube = (t_cub*)param;
	static clock_t previous_frame_time = 0;
	clock_t current_time = clock();
	double delta_time = (current_time - previous_frame_time) / (double)CLOCKS_PER_SEC;

	if (delta_time >= 1.0 / TARGET_FPS)
	{
		update_player(cube);
		ft_draw_sky_floor(cube);
		cube->threads[0].id = 0;
		cube->threads[1].id = 1;
		pthread_create(&cube->threads[0].thread, NULL, &draw_lines_3d, (void *)cube);
		pthread_create(&cube->threads[1].thread, NULL, &draw_lines_3d_1, (void *)cube);
		pthread_join(cube->threads[0].thread, NULL);
		pthread_join(cube->threads[1].thread, NULL);

		draw_per(cube);
		update_run_on_right_click(cube);
		update_y_press(cube);
		draw_gun_right_click(cube);
		previous_frame_time = current_time;
	}
	clock_t frame_time = clock() - current_time;
	if (frame_time < FRAME_TARGET_TIME)
	{
		struct timespec rem, req = {0, (FRAME_TARGET_TIME - frame_time) * 1000000};
		nanosleep(&req, &rem);
	}
	// Mouse handling
	int32_t xpos, ypos;
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && (cube->player->start == 0 || cube->player->start == 1))
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start += 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
// // end hooks
