/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:35 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:03:36 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../recasting.h"

int	has_wall(t_cub *cube, double x1, double y1, int is)
{
	int	x;
	int	y;

	if (is == 1)
		y1--;
	else if (is == 2)
		x1--;
	x = floor(x1 / TILE_SIZE);
	y = floor(y1 / TILE_SIZE);
	if (cube->data->map[y][x] == '1'
		|| cube->data->map[y][x] == 'D')
		return (1);
	return (0);
}

static void	ft_draw_her_wh(t_cub *cube, t_vars *vars, int a)
{
	int	wall;

	while (vars->nexthorztouchx > 0
		&& vars->nexthorztouchx < cube->data->map_cols * TILE_SIZE
		&& vars->nexthorztouchy > 0
		&& vars->nexthorztouchy < cube->data->map_row * TILE_SIZE)
	{
		wall = has_wall(cube, vars->nexthorztouchx, vars->nexthorztouchy, a);
		if (wall == 1)
		{
			vars->foundhorzwallhit = 1;
			vars->horzwallhitx = vars->nexthorztouchx;
			vars->horzwallhity = vars->nexthorztouchy;
			vars->door = 0;
			break ;
		}
		else
		{
			vars->nexthorztouchx += vars->xstep;
			vars->nexthorztouchy += vars->ystep;
		}
	}
}

void	ft_draw_hero(t_cub *cube, t_vars *vars)
{
	int	a;

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
	a = 0;
	if (vars->israyfacingup)
		a = 1;
	ft_draw_her_wh(cube, vars, a);
}

static void	ft_draw_ver_wh(t_cub *cube, t_vars *vars, int a)
{
	int	wall;

	while (vars->nextverttouchx > 0
		&& vars->nextverttouchx < cube->data->map_cols * TILE_SIZE
		&& vars->nextverttouchy > 0
		&& vars->nextverttouchy < cube->data->map_row * TILE_SIZE)
	{
		wall = has_wall(cube, vars->nextverttouchx, vars->nextverttouchy, a);
		if (wall == 1)
		{
			vars->foundvertwallhit = 1;
			vars->vertwallhitx = vars->nextverttouchx;
			vars->vertwallhity = vars->nextverttouchy;
			vars->door = 0;
			break ;
		}
		else
		{
			vars->nextverttouchx += vars->xstep;
			vars->nextverttouchy += vars->ystep;
		}
	}
}

void	ft_draw_ver(t_cub *cube, t_vars *vars)
{
	int	a;

	vars->foundvertwallhit = 0;
	vars->vertwallhitx = 0;
	vars->vertwallhity = 0;
	vars->x_intercept = floor(cube->player->player_x / TILE_SIZE) * TILE_SIZE;
	if (vars->israyfacingright)
		vars->x_intercept += TILE_SIZE;
	vars->y_intercept = cube->player->player_y
		+ (vars->x_intercept - cube->player->player_x) * tan(vars->angle);
	vars->xstep = TILE_SIZE;
	if (vars->israyfacingleft)
		vars->xstep *= -1;
	vars->ystep = TILE_SIZE * tan(vars->angle);
	if (vars->israyfacingup && vars->ystep > 0)
		vars->ystep *= -1;
	if (vars->israyfacingdown && vars->ystep < 0)
		vars->ystep *= -1;
	vars->nextverttouchx = vars->x_intercept;
	vars->nextverttouchy = vars->y_intercept;
	a = 0;
	if (vars->israyfacingleft)
		a = 2;
	ft_draw_ver_wh(cube, vars, a);
}
