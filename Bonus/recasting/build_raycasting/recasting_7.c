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

void	ft_calc_text_data(t_texture_data *v, t_vars vars,
	int txturnm, t_cub *cube)
{
	v->texture = NULL;
	if (vars.door)
		v->texture = cube->doors[0];
	else
		v->texture = cube->texture[txturnm];
	v->textureposx = fmod(vars.wallhitx, tile_size) / tile_size;
	if (vars.washitvert)
		v->textureposx = fmod(vars.wallhity, tile_size) / tile_size;
	v->textureposx = 1.0 - v->textureposx;
	v->texture_x = (int)(v->textureposx * v->texture->width);
	v->texture_pos = vars.textureoffsety * vars.texturestep;
	v->shade = fmax(0.2, 1.0 - (vars.distance / 500.0));
}

void	ft_get_texture_b(t_cub *cube, t_vars vars, int txturnm, int i)
{
	int				y;
	t_texture_data	v;

	ft_calc_text_data(&v, vars, txturnm, cube);
	y = vars.walltoppixel;
	while (y < vars.wallbottompixel && y < HEIGHT)
	{
		v.texture_y = (int)(v.texture_pos
				* v.texture->height) % v.texture->height;
		v.texture_pos += vars.texturestep;
		v.color = get_pixel_color(v.texture, v.texture_x, v.texture_y);
		if (!(vars.door_var
				&& ((ft_shaded_color(v.color, v.shade) & 0xFFFFFF00) == 0))
			&& y >= 0)
			mlx_put_pixel(cube->image, i, y, ft_shaded_color(v.color, v.shade));
		y++;
	}
}
