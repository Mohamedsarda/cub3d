#include "../recasting.h"

static void	ft_ver_calc(t_cub *cube, t_vars *vars)
{
	vars->foundvertwallhit = 0;
	vars->vertwallhitx = 0;
	vars->vertwallhity = 0;
	vars->x_intercept = floor(cube->player->player_x / tile_size) * tile_size;
	if (vars->israyfacingright)
		vars->x_intercept += tile_size;
	vars->y_intercept = cube->player->player_y
		+ (vars->x_intercept - cube->player->player_x) * tan(vars->angle);
	vars->xstep = tile_size;
	if (vars->israyfacingleft)
		vars->xstep *= -1;
	vars->ystep = tile_size * tan(vars->angle);
	if (vars->israyfacingup && vars->ystep > 0)
		vars->ystep *= -1;
	if (vars->israyfacingdown && vars->ystep < 0)
		vars->ystep *= -1;
	vars->nextverttouchx = vars->x_intercept;
	vars->nextverttouchy = vars->y_intercept;
}

void	ft_draw_ver(t_cub *cube, t_vars *vars)
{
	int	a;

	a = 0;
	ft_ver_calc(cube, vars);
	if (vars->israyfacingleft)
		a = 2;
	while (vars->nextverttouchx > 0
		&& vars->nextverttouchx < cube->data->map_cols * tile_size
		&& vars->nextverttouchy > 0
		&& vars->nextverttouchy < cube->data->map_row * tile_size)
	{
		if (has_wall(cube, vars->nextverttouchx, vars->nextverttouchy, a))
		{
			vars->foundvertwallhit = 1;
			vars->vertwallhitx = vars->nextverttouchx;
			vars->vertwallhity = vars->nextverttouchy;
			break ;
		}
		else
		{
			vars->nextverttouchx += vars->xstep;
			vars->nextverttouchy += vars->ystep;
		}
	}
}
