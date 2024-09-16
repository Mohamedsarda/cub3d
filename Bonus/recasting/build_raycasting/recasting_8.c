#include "../../../recasting.h"

t_vars	_open(t_cub *cube, int x, int y, double angle)
{
	t_vars	vars;

	if (cube->player->open == -1)
	{
		if (y != 0 && cube->data->map[y - 1][x] == 'D')
			cube->data->map[y - 1][x] = 'T';
		if (y != cube->data->map_row && cube->data->map[y + 1][x] == 'D')
			cube->data->map[y + 1][x] = 'T';
		if (x != 0 && cube->data->map[y][x - 1] == 'D')
			cube->data->map[y][x - 1] = 'T';
		if (x != cube->data->map_cols && cube->data->map[y][x + 1] == 'D')
			cube->data->map[y][x + 1] = 'T';
		if (y != 1 && cube->data->map[y - 2][x] == 'T')
			cube->data->map[y - 2][x] = 'D';
		if (y != cube->data->map_row - 2 && cube->data->map[y + 2][x] == 'T')
			cube->data->map[y + 2][x] = 'D';
		if (x != 1 && cube->data->map[y][x - 2] == 'T')
			cube->data->map[y][x - 2] = 'D';
		if (x != cube->data->map_cols - 2 && cube->data->map[y][x + 2] == 'T')
			cube->data->map[y][x + 2] = 'D';
	}
	vars = draw_line(cube, angle, 0);
	return (vars);
}

t_vars	_close(t_cub *cube, int x, int y, double angle)
{
	t_vars	vars;

	if (cube->player->open == 1)
	{
		if (y != 1 && cube->data->map[y - 2][x] == 'T')
			cube->data->map[y - 2][x] = 'D';
		if (y != cube->data->map_row - 2 && cube->data->map[y + 2][x] == 'T')
			cube->data->map[y + 2][x] = 'D';
		if (x != 1 && cube->data->map[y][x - 2] == 'T')
			cube->data->map[y][x - 2] = 'D';
		if (x != cube->data->map_cols - 2 && cube->data->map[y][x + 2] == 'T')
			cube->data->map[y][x + 2] = 'D';
		if (y != 1 && cube->data->map[y - 1][x - 1] == 'T')
			cube->data->map[y - 1][x - 1] = 'D';
		if (y != cube->data->map_row - 2
			&& cube->data->map[y + 1][x + 1] == 'T')
			cube->data->map[y + 1][x + 1] = 'D';
		if (x != 1 && cube->data->map[y + 1][x - 1] == 'T')
			cube->data->map[y + 1][x - 1] = 'D';
		if (x != cube->data->map_cols - 2
			&& cube->data->map[y - 1][x + 1] == 'T')
			cube->data->map[y - 1][x + 1] = 'D';
	}
	vars = draw_line(cube, angle, 0);
	return (vars);
}

t_vars	open_door(t_vars vars, t_cub *cube, double angle)
{
	double	fx;
	double	fy;
	int		x;
	int		y;

	fx = cube->player->player_x;
	fy = cube->player->player_y;
	if (vars.washitvert && vars.israyfacingleft)
		fx--;
	if (!vars.washitvert && vars.israyfacingup)
		fy--;
	x = floor(fx / tile_size);
	y = floor(fy / tile_size);
	vars = _open(cube, x, y, angle);
	vars = _close(cube, x, y, angle);
	return (vars);
}


void	*draw_lines_3d_1(void *tmp)
{
	t_cub	*cube;
	double	distance_proj_plane;
	int		i;
	t_vars	vars;

	cube = (t_cub *)tmp;
	distance_proj_plane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
	cube->angle_1 = cube->player->rotat_angle;
	i = (WIDTH / 2);
	while (i < WIDTH)
	{
		vars = draw_line(cube, cube->angle_1, 0);
		vars = open_door(vars, cube, cube->angle_1);
		init_vars_1(&vars, cube, distance_proj_plane, cube->angle_1);
		ft_get_texture_b(cube, vars, texternum(vars), i, (cube->doortype / 2));
		cube->angle_1 += (FOV_ANGLE / WIDTH);
		i++;
	}
	return (NULL);
}