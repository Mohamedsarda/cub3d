/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mini_map_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:30 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:06:27 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../recasting.h"

void	ft_draw_player_indecator_helper(t_p_indecator *v, t_cub *cube)
{
	v->indicator_length = 20;
	v->end_x = cube->player->minimap_player_x
		+ cos(cube->player->rotat_angle) * v->indicator_length;
	v->end_y = cube->player->minimap_player_y
		+ sin(cube->player->rotat_angle) * v->indicator_length;
	v->dx = abs(v->end_x - cube->player->minimap_player_x);
	v->dy = abs(v->end_y - cube->player->minimap_player_y);
	v->sx = -1;
	if (cube->player->minimap_player_x < v->end_x)
		v->sx = 1;
	v->sy = -1;
	if (cube->player->minimap_player_y < v->end_y)
		v->sy = 1;
	v->err = v->dx - v->dy;
}

void	ft_draw_player_indecator(t_cub *cube)
{
	t_p_indecator	v;

	ft_draw_player_indecator_helper(&v, cube);
	while ((int)cube->player->minimap_player_x != v.end_x
		|| (int)cube->player->minimap_player_y != v.end_y)
	{
		mlx_put_pixel(cube->image, cube->player->minimap_player_x,
			cube->player->minimap_player_y, c_rgba(255, 0, 0, 255));
		v.e2 = 2 * v.err;
		if (v.e2 > -v.dy)
		{
			v.err -= v.dy;
			cube->player->minimap_player_x += v.sx;
		}
		if (v.e2 < v.dx)
		{
			v.err += v.dx;
			cube->player->minimap_player_y += v.sy;
		}
	}
}

void	ft_draw_map_background_helper(t_map_background v, t_cub *cube)
{
	if (cube->player->minimap == 1 && cube->player->tab != 1)
	{
		if (check_drow_minimap(cube, v.dx, v.dy, 2) == 1)
			mlx_put_pixel(cube->image, v.sx, v.sy, c_rgba(0, 0, 0, 1));
	}
	else
	{
		if (check_drow_minimap(cube, v.dx, v.dy, 1) == 1)
		{
			if (check_drow_minimap_1(cube, v.x, v.y, 1) == 1)
				mlx_put_pixel(cube->image, v.sx,
					v.sy, c_rgba(0, 0, 0, 255));
			else
				mlx_put_pixel(cube->image, v.sx,
					v.sy, c_rgba(0, 0, 0, 1));
		}
	}
}

void	ft_draw_map_background(t_cub *cube)
{
	t_map_background	v;

	v.y = -(cube->player->minimap_size / 2);
	while (v.y <= (cube->player->minimap_size / 2))
	{
		v.x = -(cube->player->minimap_size / 2);
		while (v.x <= (cube->player->minimap_size / 2))
		{
			v.sx = cube->player->minimap_start_x
				+ (cube->player->minimap_size / 2) + v.x;
			v.sy = cube->player->minimap_start_y
				+ (cube->player->minimap_size / 2) + v.y;
			v.dx = v.x;
			v.dy = v.y;
			ft_draw_map_background_helper(v, cube);
			v.x++;
		}
		v.y++;
	}
}
