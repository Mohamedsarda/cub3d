#include "../../../recasting.h"


// draw_player
// t_pos ft_is_a_door(double x, double y, t_cub *cube)
// {
// 	(void)x;
// 	(void)y;
// 	t_pos p;
// 	p.x = -1;
// 	p.y = -1;
// 	while (++p.x < cube->data->map_row)
// 	{
// 		p.y = -1;
// 		while (++p.y < cube->data->map_row)
// 		{
// 			if (p.x <= cube->data->map_cols && (cube->data->map[p.x][p.y] == 'D' || cube->data->map[p.x][p.y] == 'T'))
// 				return (p);
// 		}
// 	}
// 	return (p);
// }

int	is_it_a_wall(double x, double y, t_cub *cube)
{
	double left;
	double up;
	double right;
	double down;
	int  t_left;
	int  t_up;
	int  t_right;
	int  t_down;

	left = x - ((double)cube->player->radius);
	up = y - ((double)cube->player->radius);
	right = x + ((double)cube->player->radius);
	down = y + ((double)cube->player->radius);
	if (left < 0 || right > cube->data->map_cols * tile_size
		|| up < 0 || down > cube->data->map_row * tile_size)
		return (0);
	t_left = floor(left / tile_size);
	t_up = floor(up / tile_size);
	t_right = floor(right / tile_size);
	t_down = floor(down / tile_size);
	if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
		|| cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
		return (0);
	if ((cube->data->map[t_up][t_left] == 'D' || cube->data->map[t_down][t_right] == 'D'
		|| cube->data->map[t_up][t_right] == 'D' || cube->data->map[t_down][t_left] == 'D') && cube->player->open == 1)
		return (0);
	return (1);
}

// void	ft_draw_line(t_point x1, t_point x2, t_fdf *box)
// {
// 	box->draw.dx = abs(x2.x - x1.x);
// 	box->draw.dy = abs(x2.y - x1.y);
// 	if (x1.x < x2.x)
// 		box->draw.sx = 1;
// 	else
// 		box->draw.sx = -1;
// 	if (x1.y < x2.y)
// 		box->draw.sy = 1;
// 	else
// 		box->draw.sy = -1;
// 	box->draw.error = box->draw.dx - box->draw.dy;
// 	while (1)
// 	{
// 		if (x1.x >= 0 && x1.y >= 0 && x1.x < WIDTH && x1.y < HEIGHT)
// 			my_pixel_put(x1.x, x1.y, box->var.color, box);
// 		if (x1.x == x2.x && x1.y == x2.y)
// 			break ;
// 		box->draw.e2 = 2 * box->draw.error;
// 		if (box->draw.e2 >= -box->draw.dy)
// 		{
// 			box->draw.error -= box->draw.dy;
// 			x1.x += box->draw.sx;
// 		}
// 		if (box->draw.e2 <= box->draw.dx)
// 		{
// 			box->draw.error += box->draw.dx;
// 			x1.y += box->draw.sy;
// 		}
// 	}
// }

void	DDA(t_cub *cube, double X0, double Y0, double X1, double Y1)
{
	double dx = X1 - X0;
	double dy = Y1 - Y0;
	double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	double Xinc = dx / steps;
	double Yinc = dy / steps;
	double X = X0;
	double Y = Y0;
	int i = 0;
	while (i <= steps)
	{
		mlx_put_pixel(cube->image, X , Y, c_rgba(250, 100, 100 , 255));
		X += Xinc;
		Y += Yinc;
		i++;
	}
}




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

void draw_gun_right_click(t_cub *cube)
{
	static int previous_gun_index = -1;

	int gun_index = cube->cur_g_right_clikc;

	if (previous_gun_index != -1 && cube->gun_r_img[previous_gun_index])
	{
		mlx_delete_image(cube->mlx, cube->gun_r_img[previous_gun_index]);
		cube->gun_r_img[previous_gun_index] = NULL;
	}

	if (!cube->gun_r_img[gun_index])
	{
		cube->gun_r_img[gun_index] = mlx_texture_to_image(cube->mlx, cube->gun_r[gun_index]);
		if (!cube->gun_r_img[gun_index])
			ft_error();
	}
	if (mlx_image_to_window(cube->mlx, cube->gun_r_img[gun_index], WIDTH / 2 - cube->gun_r[gun_index]->width / 2, HEIGHT - cube->gun_r[gun_index]->height) < 0)
		ft_error();

	previous_gun_index = gun_index;
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


void *draw_lines_3D(void *tmp)
{
	t_cub *cube = (t_cub *)tmp;
	double distanceProjPlane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
	double angleStep = FOV_ANGLE / WIDTH;
	cube->angle_0 = cube->player->rotat_angle - FOV_ANGLE / 2.0;

	for (int i = 0; i < WIDTH / 2; i++)
	{
		t_vars vars = draw_line(cube, cube->angle_0, 0);
		double fx = cube->player->player_x;
		double fy = cube->player->player_y;
		if (vars.washitvert && vars.israyfacingleft)
			fx--;
		if (!vars.washitvert && vars.israyfacingup)
			fy--;
		int x = floor(fx / tile_size);
		int y = floor(fy / tile_size);

		// if next qube is D open / up down right left
		if (cube->player->open == -1)
		{
			if (y != 0 && cube->data->map[y - 1][x] == 'D')
			{
				cube->data->map[y - 1][x] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
			if (y != cube->data->map_row && cube->data->map[y + 1][x] == 'D')
			{
				cube->data->map[y + 1][x] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
			if (x != 0 && cube->data->map[y][x - 1] == 'D')
			{
				cube->data->map[y][x - 1] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
			if (x != cube->data->map_cols && cube->data->map[y][x + 1] == 'D')
			{
				cube->data->map[y][x + 1] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
		}
		// end if next qube is D open / up down right left 

		// if im awaay of the door close it / up down right left
		if (y != 1 && cube->data->map[y - 2][x] == 'T')
		{
			cube->data->map[y - 2][x] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 2][x] == 'T')
		{
			cube->data->map[y + 2][x] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != 1 && cube->data->map[y][x - 2] == 'T')
		{
			cube->data->map[y][x - 2] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y][x + 2] == 'T')
		{
			cube->data->map[y][x + 2] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		// end if im awaay of the door close it / up down right left

		// if im awaay of the door close it / up-right down-right up-left down-left
		if (y != 1 && cube->data->map[y - 1][x - 1] == 'T')
		{
			cube->data->map[y - 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 1][x + 1] == 'T')
		{
			cube->data->map[y + 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != 1 && cube->data->map[y + 1][x - 1] == 'T')
		{
			cube->data->map[y + 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y - 1][x + 1] == 'T')
		{
			cube->data->map[y - 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		// end if im awaay of the door close it / up-right down-right up-left down-left
		
		double wallDistance = vars.distance * cos(cube->angle_0 - cube->player->rotat_angle);
		double wallStripHeight = (tile_size / wallDistance) * distanceProjPlane;

		vars.walltoppixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var;
		vars.wallbottompixel = fmin((HEIGHT / 2.0) + (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var, HEIGHT);

		vars.texturestep = 1.0 / wallStripHeight;
		vars.textureoffsety = 0;

		int textureNum = vars.washitvert ?
			(vars.israyfacingleft ? 2 : 3) :
			(vars.israyfacingup ? 0 : 1);
		ft_get_texture(cube, vars, textureNum, i, (cube->doortype / 2));
		cube->angle_0 += angleStep;
	}
	return (NULL);
}


// hooks



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
	static double last_gun_change_time = 0;
	double current_time = mlx_get_time();

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
	// inti_jump(cube);
	// printf("%f\n", cube->player->jump);
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

// heal
// void	draw_inside_head(t_cub *cube)
// {
// 	int	i;
// 	int	y;
// 	i = 0;
// 	while (i <= 400)
// 	{
// 		y = 0;
// 		while (y <= 20)
// 		{
// 			if (i != 0 && y != 0 && y != 20 && i != 400)
// 				mlx_put_pixel(cube->image, i + 10,
// 					y + 10, c_rgba(0, 0, 255, 255));
// 			y++;
// 		}
// 		i++;
// 	}
// }

// void	heal_bar(t_cub *cube)
// {
// 	int	i;
// 	int	y;
// 	i = 0;
// 	while (i <= 400)
// 	{
// 		y = 0;
// 		while (y <= 20)
// 		{
// 			if (i == 0 || y == 0 || y == 20 || i == 400)
// 				mlx_put_pixel(cube->image, i + 10,
// 					y + 10, c_rgba(255, 0, 0, 255));
// 			y++;
// 		}
// 		i++;
// 	}
// 	draw_inside_head(cube);
// }
// end heal

// shots
// void	draw_shots(t_cub *cube)
// {
// 	int	i;
// 	int	y;
// 	i = 0;
// 	while (i <= 280)
// 	{
// 		y = 0;
// 		while (y <= 150)
// 		{
// 			if (i == 135)
// 				mlx_put_pixel(cube->image, i + WIDTH / 1.2,
// 					y + HEIGHT / 1.2, c_rgba(0, 0, 255, 255));
// 			else
// 				mlx_put_pixel(cube->image, i + WIDTH / 1.2,
// 					y + HEIGHT / 1.2, c_rgba(0, 0, 0, 255));
// 			y++;
// 		}
// 		i++;
// 	}
// }
// end shots

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
		pthread_create(&cube->threads[0].thread, NULL, &draw_lines_3D, (void *)cube);
		pthread_create(&cube->threads[1].thread, NULL, &draw_lines_3D_1, (void *)cube);
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
