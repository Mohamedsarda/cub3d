#include "../../../recasting.h"

void	ft_check_y_gun(t_cub *cube, int prev_gun_index)
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
}

void	update_y_press(t_cub *cube)
{
	static int	prev_gun_index;

	if (cube->y_press)
	{
		ft_check_y_gun(cube, prev_gun_index);
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
	t_mouse_data	v;

	v.prev_xpos = WIDTH / 2;
	v.sensitivity = 0.001;
	mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
	mlx_get_mouse_pos(cube->mlx, &v.xpos, &v.ypos);
	v.delta_x = v.xpos - v.prev_xpos;
	cube->player->rotat_angle += v.delta_x * v.sensitivity;
	mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
	v.prev_ypos = HEIGHT / 2;
	v.delta_y = v.ypos - v.prev_ypos;
	cube->player->player_z = cube->player->player_z + v.delta_y;
	if (cube->player->player_z > 1000)
		cube->player->player_z = 1000;
	if (cube->player->player_z < -1000)
		cube->player->player_z = -1000;
}
