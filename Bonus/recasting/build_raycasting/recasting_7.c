#include "../../../recasting.h"

void	my_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub	*cube;

	cube = param;
	if (keydata.action == MLX_PRESS)
		(ft_press(cube, keydata), ft_press_1(cube, keydata));
	if (keydata.action == MLX_RELEASE)
	{
		ft_realse(cube, keydata);
		if (keydata.key == MLX_KEY_Y)
		{
			cube->t_press = 0;
			if (cube->gun_img[cube->current_gun_index])
			{
				mlx_delete_image(cube->mlx,
					cube->gun_img[cube->current_gun_index]);
				cube->gun_img[cube->current_gun_index] = NULL;
			}
			if (cube->gun_img[0])
			{
				mlx_delete_image(cube->mlx, cube->gun_img[0]);
				cube->gun_img[0] = NULL;
			}
		}
	}
}

void	update_run_on_right_click(t_cub *cube)
{
	static double	last_gun_change_time;
	double			current_time;

	last_gun_change_time = 0;
	current_time = mlx_get_time();
	if (cube->right_press)
	{
		if ((current_time - last_gun_change_time) > 0.15)
		{
			if (cube->gun_r_img[cube->cur_g_right_clikc])
			{
				mlx_delete_image(cube->mlx,
					cube->gun_r_img[cube->cur_g_right_clikc]);
				cube->gun_r_img[cube->cur_g_right_clikc] = NULL;
			}
			cube->cur_g_right_clikc++;
			if (cube->cur_g_right_clikc > 1)
				cube->cur_g_right_clikc = 0;
			last_gun_change_time = current_time;
		}
	}
	else
		cube->cur_g_right_clikc = 0;
}

int	calculate_distance(int x1, int y1, int x2, int y2)
{
	int	delta_x;
	int	delta_y;

	delta_x = x2 - x1;
	delta_y = y2 - y1;
	return (sqrt(delta_x * delta_x + delta_y * delta_y));
}

void	draw_inside_head(t_cub *cube)
{
	int	i;
	int	y;

	i = 0;
	while (i <= 400)
	{
		y = 0;
		while (y <= 20)
		{
			if (i != 0 && y != 0 && y != 20 && i != 400)
				mlx_put_pixel(cube->image, i + 10,
					y + 10, c_rgba(0, 0, 255, 255));
			y++;
		}
		i++;
	}
}

void	ft_get_texture_b(t_cub *cube, t_vars vars, int txturnm, int i, int door)
{
	mlx_texture_t	*texture;
	double			texturePosX;
	int				textureX;
	double			texturePos;
	double			shade;

	texture = NULL;
	if (vars.door)
		texture = cube->doors[0];
	else
		texture = cube->texture[txturnm];
	texturePosX = vars.washitvert ? fmod(vars.wallhity, tile_size) / tile_size : fmod(vars.wallhitx, tile_size) / tile_size;
	texturePosX = 1.0 - texturePosX;
	textureX = (int)(texturePosX * texture->width);
	texturePos = vars.textureoffsety * vars.texturestep;
	shade = fmax(0.2, 1.0 - (vars.distance / 500.0));

	for (int y = vars.walltoppixel; y < vars.wallbottompixel && y < HEIGHT; y++)
	{
		int textureY = (int)(texturePos * texture->height) % texture->height;
		texturePos += vars.texturestep;
		uint32_t color = get_pixel_color(texture, textureX, textureY);
		if (!(door && ((ft_shaded_color(color, shade) & 0xFFFFFF00) == 0)) && y >= 0)
			mlx_put_pixel(cube->image, i, y, ft_shaded_color(color, shade));
	}
}
