#include "../recasting.h"

uint32_t	ft_shade(uint32_t color, double shade)
{
	uint8_t			r;
	uint8_t			g;
	uint8_t			b;

	r = ((color >> 24) & 0xFF) * shade;
	g = ((color >> 16) & 0xFF) * shade;
	b = ((color >> 8) & 0xFF) * shade;
	return ((r << 24) | (g << 16) | (b << 8) | (color & 0xFF));
}

void	ft_get_texture_m(t_cub *cube, t_vars vars, int textureNum, int i)
{
	t_text_vars	v;
	int			y;
	uint32_t	color;
	uint32_t	shaded_color;

	v.texture = cube->texture[textureNum];
	v.texture_pos_x = fmod(vars.wallhitx, tile_size) / tile_size;
	if (vars.washitvert)
		v.texture_pos_x = fmod(vars.wallhity, tile_size) / tile_size;
	v.texture_pos_x = 1.0 - v.texture_pos_x;
	v.texture_x = (int)(v.texture_pos_x * v.texture->width);
	v.texture_pos = vars.textureoffsety * vars.texturestep;
	v.shade = fmax(0.3, 1.0 - (vars.distance / 1000.0));
	y = vars.walltoppixel;
	while (y < vars.wallbottompixel && y < HEIGHT)
	{
		v.texture_y = (int)(v.texture_pos
				* v.texture->height) % v.texture->height;
		v.texture_pos += vars.texturestep;
		color = get_pixel_color(v.texture, v.texture_x, v.texture_y);
		shaded_color = ft_shade(color, v.shade);
		if (!(vars.door && ((shaded_color & 0xFFFFFF00) == 0)) && y >= 0)
			mlx_put_pixel(cube->image, i, y, shaded_color);
		y++;
	}
}

int	ft_texture_num(t_vars vars)
{
	int	texturenum;

	if (vars.washitvert)
	{
		if (vars.israyfacingleft)
			texturenum = 2;
		else
			texturenum = 3;
	}
	else
	{
		if (vars.israyfacingup)
			texturenum = 0;
		else
			texturenum = 1;
	}
	return (texturenum);
}

void	draw_lines_3d(t_cub *cube)
{
	int			i;
	t_vars_3d	v;
	t_vars		vars;
	int			texturenum;

	ft_draw_sky_floor(cube);
	v.distanceprojplane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
	v.angle = cube->player->rotat_angle - FOV_ANGLE / 2.0;
	i = -1;
	while (++i < WIDTH)
	{
		vars = draw_line(cube, v.angle, 0);
		v.walldistance = vars.distance
			* cos(v.angle - cube->player->rotat_angle);
		v.wallstripheight = (tile_size / v.walldistance) * v.distanceprojplane;
		vars.walltoppixel = (HEIGHT / 2.0) - (v.wallstripheight / 2.0)
			- cube->player->player_z - cube->player->jump_var;
		vars.wallbottompixel = fmin((HEIGHT / 2.0) + (v.wallstripheight / 2.0)
				- cube->player->player_z - cube->player->jump_var, HEIGHT);
		vars.texturestep = 1.0 / v.wallstripheight;
		vars.textureoffsety = 0;
		texturenum = ft_texture_num(vars);
		ft_get_texture_m(cube, vars, texturenum, i);
		v.angle += FOV_ANGLE / WIDTH;
	}
}
