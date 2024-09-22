/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DrawHero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:04:08 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:04:09 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../recasting.h"

int	c_rgba(int r, int g, int b, int a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

static void	ft_hero_calc(t_cub *cube, t_vars *vars)
{
	vars->y_intercept = floor(cube->player->player_y / TILE_SIZE) * TILE_SIZE;
	if (vars->israyfacingdown)
		vars->y_intercept += TILE_SIZE;
	vars->x_intercept = cube->player->player_x
		+ (vars->y_intercept - cube->player->player_y) / tan(vars->angle);
	vars->ystep = TILE_SIZE;
	if (vars->israyfacingup)
		vars->ystep *= -1;
	vars->xstep = TILE_SIZE / tan(vars->angle);
	if (vars->israyfacingleft && vars->xstep > 0)
		vars->xstep *= -1;
	if (vars->israyfacingright && vars->xstep < 0)
		vars->xstep *= -1;
	vars->nexthorztouchx = vars->x_intercept;
	vars->nexthorztouchy = vars->y_intercept;
}

void	ft_draw_hero(t_cub *cube, t_vars *vars)
{
	int	a;

	a = 0;
	ft_hero_calc(cube, vars);
	if (vars->israyfacingup)
		a = 1;
	while (vars->nexthorztouchx > 0
		&& vars->nexthorztouchx < cube->data->map_cols * TILE_SIZE
		&& vars->nexthorztouchy > 0
		&& vars->nexthorztouchy < cube->data->map_row * TILE_SIZE)
	{
		if (has_wall(cube, vars->nexthorztouchx, vars->nexthorztouchy, a))
		{
			vars->foundhorzwallhit = 1;
			vars->horzwallhitx = vars->nexthorztouchx;
			vars->horzwallhity = vars->nexthorztouchy;
			break ;
		}
		else
		{
			vars->nexthorztouchx += vars->xstep;
			vars->nexthorztouchy += vars->ystep;
		}
	}
}
