#include "../../recasting.h"

int	has_wall(t_cub *cube, double x1, double y1, int is)
{
	int	x;
	int	y;

	if (is == 1)
		y1--;
	else if (is == 2)
		x1--;
	x = floor(x1 / tile_size);
	y = floor(y1 / tile_size);
	if (cube->data->map[y][x] == '1'
		|| cube->data->map[y][x] == 'D' || cube->data->map[y][x] == 'O')
		return (1);
	return (0);
}

void	ft_draw_her_wh(t_cub *cube, t_vars *vars, int a)
{
	int	wall;

	while (vars->nextHorzTouchX > 0
		&& vars->nextHorzTouchX < cube->data->map_cols * tile_size
		&& vars->nextHorzTouchY > 0
		&& vars->nextHorzTouchY < cube->data->map_row * tile_size)
	{
		wall = has_wall(cube, vars->nextHorzTouchX, vars->nextHorzTouchY, a);
		if (wall == 1)
		{
			vars->foundHorzWallHit = 1;
			vars->horzWallHitX = vars->nextHorzTouchX;
			vars->horzWallHitY = vars->nextHorzTouchY;
			vars->door = 0;
			break ;
		}
		else
		{
			vars->nextHorzTouchX += vars->xstep;
			vars->nextHorzTouchY += vars->ystep;
		}
	}
}

void	ft_draw_hero(t_cub *cube, t_vars *vars)
{
	int	a;

	vars->y_intercept = floor(cube->player->player_y / tile_size) * tile_size;
	if (vars->isRayFacingDown)
		vars->y_intercept += tile_size;
	vars->x_intercept = cube->player->player_x
		+ (vars->y_intercept - cube->player->player_y) / tan(vars->angle);
	vars->ystep = tile_size;
	if (vars->isRayFacingUp)
		vars->ystep *= -1;
	vars->xstep = tile_size / tan(vars->angle);
	if (vars->isRayFacingLeft && vars->xstep > 0)
		vars->xstep *= -1;
	if (vars->isRayFacingRight && vars->xstep < 0)
		vars->xstep *= -1;
	vars->nextHorzTouchX = vars->x_intercept;
	vars->nextHorzTouchY = vars->y_intercept;
	a = 0;
	if (vars->isRayFacingUp)
		a = 1;
	ft_draw_her_wh(cube, vars, a);
}

void	ft_draw_ver_wh(t_cub *cube, t_vars *vars, int a)
{
	int	wall;

	while (vars->nextVertTouchX > 0
		&& vars->nextVertTouchX < cube->data->map_cols * tile_size
		&& vars->nextVertTouchY > 0
		&& vars->nextVertTouchY < cube->data->map_row * tile_size)
	{
		wall = has_wall(cube, vars->nextVertTouchX, vars->nextVertTouchY, a);
		if (wall == 1)
		{
			vars->foundVertWallHit = 1;
			vars->vertWallHitX = vars->nextVertTouchX;
			vars->vertWallHitY = vars->nextVertTouchY;
			vars->door = 0;
			break ;
		}
		else
		{
			vars->nextVertTouchX += vars->xstep;
			vars->nextVertTouchY += vars->ystep;
		}
	}
}

void	ft_draw_ver(t_cub *cube, t_vars *vars)
{
	int	a;

	vars->foundVertWallHit = 0;
	vars->vertWallHitX = 0;
	vars->vertWallHitY = 0;
	vars->x_intercept = floor(cube->player->player_x / tile_size) * tile_size;
	if (vars->isRayFacingRight)
		vars->x_intercept += tile_size;
	vars->y_intercept = cube->player->player_y
		+ (vars->x_intercept - cube->player->player_x) * tan(vars->angle);
	vars->xstep = tile_size;
	if (vars->isRayFacingLeft)
		vars->xstep *= -1;
	vars->ystep = tile_size * tan(vars->angle);
	if (vars->isRayFacingUp && vars->ystep > 0)
		vars->ystep *= -1;
	if (vars->isRayFacingDown && vars->ystep < 0)
		vars->ystep *= -1;
	vars->nextVertTouchX = vars->x_intercept;
	vars->nextVertTouchY = vars->y_intercept;
	a = 0;
	if (vars->isRayFacingLeft)
		a = 2;
	ft_draw_ver_wh(cube, vars, a);
}

double	distanceBetweenPoints(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	ft_get_close_wall(t_cub *cube, t_vars *vars)
{
	vars->horzHitDistance = __INT_MAX__;
	vars->vertHitDistance = __INT_MAX__;
	if (vars->foundHorzWallHit)
		vars->horzHitDistance = distanceBetweenPoints(cube->player->player_x,
				cube->player->player_y, vars->horzWallHitX, vars->horzWallHitY);
	if (vars->foundVertWallHit)
		vars->vertHitDistance = distanceBetweenPoints(cube->player->player_x,
				cube->player->player_y, vars->vertWallHitX, vars->vertWallHitY);
	vars->wallHitX = vars->vertWallHitX;
	vars->wallHitY = vars->vertWallHitY;
	vars->distance = vars->vertHitDistance;
	if (vars->vertHitDistance < vars->horzHitDistance)
	{
		vars->wallHitX = vars->vertWallHitX;
		vars->wallHitY = vars->vertWallHitY;
		vars->distance = vars->vertHitDistance;
		vars->wasHitVert = 1;
	}
	else
	{
		vars->wallHitX = vars->horzWallHitX;
		vars->wallHitY = vars->horzWallHitY;
		vars->distance = vars->horzHitDistance;
		vars->wasHitVert = 0;
	}
}

void	check_if_door(t_cub *cube, t_vars *vars)
{
	double	fx;
	double	fy;
	int		x;
	int		y;

	fx = vars->wallHitX;
	fy = vars->wallHitY;
	if (vars->isRayFacingLeft)
		fx--;
	if (vars->isRayFacingUp)
		fy--;
	x = floor(fx / tile_size);
	y = floor(fy / tile_size);
	if (cube->data->map[y][x] == 'D')
		vars->door = 1;
}

t_vars	draw_line(t_cub *cube, double angle, int is)
{
	t_vars	vars;

	(void)is;
	vars.angle = normalizeAngle(angle);
	vars.isRayFacingDown = vars.angle > 0 && vars.angle < M_PI;
	vars.isRayFacingUp = !vars.isRayFacingDown;
	vars.isRayFacingRight = vars.angle < 0.5 * M_PI || vars.angle > 1.5 * M_PI;
	vars.isRayFacingLeft = !vars.isRayFacingRight;
	vars.foundHorzWallHit = 0;
	vars.horzWallHitX = 0;
	vars.horzWallHitY = 0;
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
