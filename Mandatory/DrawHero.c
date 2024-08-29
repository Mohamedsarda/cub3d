#include "../recasting.h"

static void	ft_hero_calc(t_cub *cube, t_vars *vars)
{
	vars->y_intercept = floor(cube->player->player_y / tile_size) * tile_size;
	if (vars->israyfacingdown)
		vars->y_intercept += tile_size;
	vars->x_intercept = cube->player->player_x
		+ (vars->y_intercept - cube->player->player_y) / tan(vars->angle);
	vars->ystep = tile_size;
	if (vars->israyfacingup)
		vars->ystep *= -1;
	vars->xstep = tile_size / tan(vars->angle);
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
		&& vars->nexthorztouchx < cube->data->map_cols * tile_size
		&& vars->nexthorztouchy > 0
		&& vars->nexthorztouchy < cube->data->map_row * tile_size)
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
