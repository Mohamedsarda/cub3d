#include "../recasting.h"

int	is_it_a_wall(double x, double y, t_cub *cube)
{
	double	left;
	double	up;
	double	right;
	double	down;

	left = x - ((double)cube->player->radius);
	up = y - ((double)cube->player->radius);
	right = x + ((double)cube->player->radius);
	down = y + ((double)cube->player->radius);
	if (left < 0 || right > cube->data->map_cols * TILE_SIZE
		|| up < 0 || down > cube->data->map_row * TILE_SIZE)
		return (0);
	left = floor(left / TILE_SIZE);
	up = floor(up / TILE_SIZE);
	right = floor(right / TILE_SIZE);
	down = floor(down / TILE_SIZE);
	if (cube->data->map[(int)up][(int)left] == '1'
		|| cube->data->map[(int)down][(int)right] == '1'
		|| cube->data->map[(int)up][(int)right] == '1'
		|| cube->data->map[(int)down][(int)left] == '1')
		return (0);
	return (1);
}

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
	if (cube->data->map[y][x] == '1' || cube->data->map[y][x] == 'D')
		return (1);
	return (0);
}

double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}
