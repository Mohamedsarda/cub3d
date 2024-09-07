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
	if (left < 0 || right > cube->data->map_cols * tile_size
		|| up < 0 || down > cube->data->map_row * tile_size)
		return (0);
	left = floor(left / tile_size);
	up = floor(up / tile_size);
	right = floor(right / tile_size);
	down = floor(down / tile_size);
	if (cube->data->map[(int)up][(int)left] == '1'
		|| cube->data->map[(int)down][(int)right] == '1'
		|| cube->data->map[(int)up][(int)right] == '1'
		|| cube->data->map[(int)down][(int)left] == '1')
		return (0);
	return (1);
}

<<<<<<< HEAD
=======
void	DDA(t_cub *cube, double X0, double Y0, double X1, double Y1)
{
	double	dx = X1 - X0;
	double	dy = Y1 - Y0;
	double	steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	double	Xinc = dx / steps;
	double	Yinc = dy / steps;
	double	X = X0;
	double	Y = Y0;
	int i;

	i = 0;
	while (i <= steps)
	{
		mlx_put_pixel(cube->image, X, Y, c_rgba(250, 100, 100, 255));
		X += Xinc;
		Y += Yinc;
		i++;
	}
}

>>>>>>> main
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
