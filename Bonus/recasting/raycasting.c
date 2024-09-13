#include "../../recasting.h"

double	distancebetween_points(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	ft_get_close_wall(t_cub *cube, t_vars *vars)
{
	vars->horzhitdistance = __INT_MAX__;
	vars->verthitdistance = __INT_MAX__;
	if (vars->foundhorzwallhit)
		vars->horzhitdistance = distancebetween_points(cube->player->player_x,
				cube->player->player_y, vars->horzwallhitx, vars->horzwallhity);
	if (vars->foundvertwallhit)
		vars->verthitdistance = distancebetween_points(cube->player->player_x,
				cube->player->player_y, vars->vertwallhitx, vars->vertwallhity);
	vars->wallhitx = vars->vertwallhitx;
	vars->wallhity = vars->vertwallhity;
	vars->distance = vars->verthitdistance;
	if (vars->verthitdistance < vars->horzhitdistance)
	{
		vars->wallhitx = vars->vertwallhitx;
		vars->wallhity = vars->vertwallhity;
		vars->distance = vars->verthitdistance;
		vars->washitvert = 1;
	}
	else
	{
		vars->wallhitx = vars->horzwallhitx;
		vars->wallhity = vars->horzwallhity;
		vars->distance = vars->horzhitdistance;
		vars->washitvert = 0;
	}
}

void	check_if_door(t_cub *cube, t_vars *vars)
{
	double	fx;
	double	fy;
	int		x;
	int		y;

	fx = vars->wallhitx;
	fy = vars->wallhity;
	if (!vars->washitvert && vars->israyfacingup)
		fy--;
	if (vars->washitvert && vars->israyfacingleft)
		fx--;
	x = floor(fx / tile_size);
	y = floor(fy / tile_size);
	if (cube->data->map[y][x] == 'D' )
		vars->door = 1;
}

t_vars	draw_line(t_cub *cube, double angle, int is)
{
	t_vars	vars;

	(void)is;
	vars.angle = normalize_angle(angle);
	vars.israyfacingdown = vars.angle > 0 && vars.angle < M_PI;
	vars.israyfacingup = !vars.israyfacingdown;
	vars.israyfacingright = vars.angle < 0.5 * M_PI || vars.angle > 1.5 * M_PI;
	vars.israyfacingleft = !vars.israyfacingright;
	vars.foundhorzwallhit = 0;
	vars.horzwallhitx = 0;
	vars.horzwallhity = 0;
	vars.door = 0;
	ft_draw_hero(cube, &vars);
	ft_draw_ver(cube, &vars);
	ft_get_close_wall(cube, &vars);
	check_if_door(cube, &vars);
	return (vars);
}

void	draw_lines(t_cub *cube, int is)
{
	double	angle;
	int		i;

	cube->is = MAP_SCALE;
	angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
	i = 0;
	while (i < NUM_RAYS)
	{
		draw_line(cube, angle, is);
		angle -= FOV_ANGLE / NUM_RAYS;
		i++;
	}
}
