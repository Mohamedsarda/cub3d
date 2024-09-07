#include "../recasting.h"

static void	ft_get_wallhit(t_vars *vars)
{
	vars->wallhitx = vars->vertwallhitx;
	vars->wallhity = vars->vertwallhity;
	vars->distance = vars->verthitdistance;
	vars->washitvert = 1;
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

static void	ft_init_rays(t_vars *vars)
{
	vars->israyfacingdown = vars->angle > 0 && vars->angle < M_PI;
	vars->israyfacingup = !vars->israyfacingdown;
	vars->israyfacingright = vars->angle < 0.5 * M_PI
		|| vars->angle > 1.5 * M_PI;
	vars->israyfacingleft = !vars->israyfacingright;
	vars->foundhorzwallhit = 0;
	vars->horzwallhitx = 0;
	vars->horzwallhity = 0;
}

static void	ft_horz_or_ver(t_vars *vars, t_cub *cube)
{
	vars->horzhitdistance = __INT_MAX__;
	vars->verthitdistance = __INT_MAX__;
	if (vars->foundhorzwallhit)
		vars->horzhitdistance = distance_between_points(cube->player->player_x,
				cube->player->player_y, vars->horzwallhitx, vars->horzwallhity);
	if (vars->foundvertwallhit)
		vars->verthitdistance = distance_between_points(cube->player->player_x,
				cube->player->player_y, vars->vertwallhitx, vars->vertwallhity);
}

t_vars	draw_line(t_cub *cube, double angle, int is)
{
	t_vars	vars;
	int		x;
	int		y;

	(void)is;
	vars.angle = normalize_angle(angle);
	ft_init_rays(&vars);
	ft_draw_hero(cube, &vars);
	ft_draw_ver(cube, &vars);
	ft_horz_or_ver(&vars, cube);
	ft_get_wallhit(&vars);
	x = floor(vars.wallhitx / tile_size);
	y = floor(vars.wallhity / tile_size);
	vars.door = 0;
	if (cube->data->map[y][x] == 'D')
		vars.door = 1;
	return (vars);
}
