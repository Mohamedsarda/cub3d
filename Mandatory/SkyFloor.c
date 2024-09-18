/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SkyFloor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:04:20 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:04:21 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../recasting.h"

uint32_t	get_pixel_color(mlx_texture_t *texture, int x, int y)
{
	t_pixel_vars	v;
	int				index;

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return (0);
	index = (y * texture->width + x) * 4;
	v.pixels = texture->pixels;
	v.r = v.pixels[index];
	v.g = v.pixels[index + 1];
	v.b = v.pixels[index + 2];
	v.a = v.pixels[index + 3];
	return ((v.r << 24) | (v.g << 16) | (v.b << 8) | v.a);
}

void	ft_draw_sky_floor_1(t_cub *cube, int i,
	int sky_end_y, int floor_start_y)
{
	int	j;

	j = 0;
	while (j < HEIGHT)
	{
		if (j < sky_end_y)
			mlx_put_pixel(cube->image, i, j,
				c_rgba(cube->data->sky.r,
					cube->data->sky.g, cube->data->sky.b, 255));
		else if (j >= floor_start_y)
			mlx_put_pixel(cube->image, i, j,
				c_rgba(cube->data->floor.r,
					cube->data->floor.g, cube->data->floor.b, 255));
		++j;
	}
}

void	ft_draw_sky_floor(t_cub *cube)
{
	int	i;
	int	sky_end_y;
	int	floor_start_y;

	sky_end_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;
	floor_start_y = HEIGHT / 2 - cube->player->player_z
		- cube->player->jump_var;
	if (sky_end_y < 0)
		sky_end_y = 0;
	if (floor_start_y >= HEIGHT)
		floor_start_y = HEIGHT - 1;
	i = -1;
	while (++i < WIDTH)
		ft_draw_sky_floor_1(cube, i, sky_end_y, floor_start_y);
}
