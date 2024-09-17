#include "../../../recasting.h"

uint32_t	ft_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

uint32_t	get_pixel_color(mlx_texture_t *texture, int x, int y)
{
	uint8_t	*pixels;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return (0);
	pixels = texture->pixels;
	r = pixels[(int)((y * texture->width + x) * 4)];
	g = pixels[(int)((y * texture->width + x) * 4) + 1];
	b = pixels[(int)((y * texture->width + x) * 4) + 2];
	a = pixels[(int)((y * texture->width + x) * 4) + 3];
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

void	ft_draw_sky_floor(t_cub *cube)
{
	int	i;
	int	j;
	int	sky_end_y;
	int	floor_st_y;

	sky_end_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;
	floor_st_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;
	if (sky_end_y < 0)
		sky_end_y = 0;
	if (floor_st_y >= HEIGHT)
		floor_st_y = HEIGHT - 1;
	i = -1;
	while (++i < WIDTH)
	{
		j = -1;
		while (++j < HEIGHT)
		{
			if (j < sky_end_y)
				mlx_put_pixel(cube->image, i, j, c_rgba(cube->data->sky.r,
						cube->data->sky.g, cube->data->sky.b, 255));
			else if (j >= floor_st_y)
				mlx_put_pixel(cube->image, i, j, c_rgba(cube->data->floor.r,
						cube->data->floor.g, cube->data->floor.b, 255));
		}
	}
}

uint32_t	ft_shaded_color(uint32_t color, double shade)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = ((color >> 24) & 0xFF) * shade;
	g = ((color >> 16) & 0xFF) * shade;
	b = ((color >> 8) & 0xFF) * shade;
	return ((r << 24) | (g << 16) | (b << 8) | (color & 0xFF));
}

t_doors	*ft_get_smallest_dist(t_doors *head)
{
	t_doors	*low;

	low = head;
	while (head)
	{
		if (head->next && head->next->distance < low->distance)
			low = head->next;
		head = head->next;
	}
	return (low);
}
