#include "../../../recasting.h"

void	draw_textured_floor(t_cub *cube)
{
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	double		pos_z;
	double		row_distance;
	double		step_x;
	double		step_y;
	double		floor_x;
	double		floor_y;
	int			cell_x;
	int			cell_y;
	int			tx;
	int			ty;
	int			p;
	uint32_t	color;
	int			y;
	int			x;

	y = HEIGHT / 2 + 1;
	dir_x = cos(cube->player->rotat_angle);
	dir_y = sin(cube->player->rotat_angle);

	plane_x = -dir_y;
	plane_y = dir_x;

	while (y < HEIGHT)
	{
		p = y - (HEIGHT / 2 + cube->player->player_z + cube->player->jump_var);

		pos_z = 0.5 * HEIGHT;

		row_distance = pos_z / p;

		step_x = row_distance * (dir_x + plane_x) / WIDTH;
		step_y = row_distance * (dir_y + plane_y) / WIDTH;

		floor_x = cube->player->player_x + row_distance * (dir_x - plane_x);
		floor_y = cube->player->player_y + row_distance * (dir_y - plane_y);

		x = 0;
		while (x < WIDTH)
		{
			cell_x = (int)(floor_x);
			cell_y = (int)(floor_y);

			tx = (int)(cube->texture[0]->width
					* (floor_x - cell_x)) & (cube->texture[0]->width - 1);
			ty = (int)(cube->texture[0]->height
					* (floor_y - cell_y)) & (cube->texture[0]->height - 1);

			floor_x += step_x;
			floor_y += step_y;

			color = get_pixel_color(cube->texture[0], tx, ty);
			mlx_put_pixel(cube->image, x, y, color);
			++x;
		}
		y++;
	}
}

void	update_y_press(t_cub *cube)
{
	static int	prev_gun_index;

	if (cube->y_press)
	{
		if (prev_gun_index != -1 && cube->gun_img[prev_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);
			cube->gun_img[prev_gun_index] = NULL;
		}
		if (!cube->gun_img[cube->current_gun_index])
		{
			cube->gun_img[cube->current_gun_index]
				= mlx_texture_to_image(cube->mlx,
					cube->gun[cube->current_gun_index]);
			if (!cube->gun_img[cube->current_gun_index])
				ft_error();
		}
		if (mlx_image_to_window(cube->mlx,
				cube->gun_img[cube->current_gun_index],
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