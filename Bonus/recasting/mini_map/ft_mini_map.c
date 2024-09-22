/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:32 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:03:33 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../recasting.h"

int	check_drow_minimap(t_cub *cube, int dx, int dy, int is)
{
	if (is == 1)
	{
		if (dx >= -(cube->player->minimap_size / 2)
			&& dx <= (cube->player->minimap_size / 2)
			&& dy >= -(cube->player->minimap_size / 2)
			&& dy <= (cube->player->minimap_size / 2))
			return (1);
	}
	else if (is == 2)
	{
		if (dx * dx + dy * dy
			<= (cube->player->minimap_size / 2)
			* (cube->player->minimap_size / 2))
			return (1);
	}
	return (0);
}

int	check_drow_minimap_1(t_cub *cube, int x, int y, int is)
{
	if (is == 1)
	{
		if (y == -(cube->player->minimap_size / 2)
			|| x == -(cube->player->minimap_size / 2)
			|| x == (cube->player->minimap_size / 2)
			|| y == (cube->player->minimap_size / 2))
			return (1);
	}
	else if (is == 1)
	{
		if (y == -(cube->player->minimap_size / 2)
			|| x == -(cube->player->minimap_size / 2)
			|| x == (cube->player->minimap_size / 2)
			|| y == (cube->player->minimap_size / 2)
			|| y == -(cube->player->minimap_size / 2) + 1
			|| x == -(cube->player->minimap_size / 2) + 1
			|| x == (cube->player->minimap_size / 2) - 1
			|| y == (cube->player->minimap_size / 2) - 1)
			return (1);
	}
	return (0);
}

void	ft_draw_player(t_cub *cube)
{
	int	player_radius;
	int	j;
	int	i;

	player_radius = 2;
	j = -player_radius;
	while (j <= player_radius)
	{
		i = -player_radius;
		while (i <= player_radius)
		{
			if (i * i + j * j <= player_radius * player_radius)
				mlx_put_pixel(cube->image,
					cube->player->minimap_player_x + i * 2,
					cube->player->minimap_player_y + j * 2,
					c_rgba(255, 0, 0, 255));
			i++;
		}
		j++;
	}
}

void	draw_minimap(t_cub *cube)
{
	int	m_s;

	cube->player->minimap_start_y = 10;
	if (cube->player->minimap == 1 && cube->player->tab != 1)
	{
		cube->player->minimap_size = 200;
		cube->player->minimap_start_x = WIDTH / 1.12;
	}
	else
	{
		cube->player->minimap_size = 500;
		cube->player->minimap_start_x = WIDTH / 1.35;
	}
	m_s = cube->player->minimap_size;
	ft_draw_map_background(cube);
	ft_draw_minimap_to_screen(cube);
	cube->player->minimap_player_x = cube->player->minimap_start_x + (m_s / 2);
	cube->player->minimap_player_y = cube->player->minimap_start_y + (m_s / 2);
	ft_draw_player(cube);
	ft_draw_player_indecator(cube);
	ft_draw_border(cube);
}

void	draw_per(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
			if (cube->data->map[y][x] == cube->data->p)
				draw_minimap(cube);
	}
}
