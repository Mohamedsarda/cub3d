#include "../../../recasting.h"

void	ft_draw_player_indecator(t_cub *cube)
{
	int	indicator_length;
	int	end_x;
	int	end_y;
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	indicator_length = 20;
	end_x = cube->player->minimap_player_x + cos(cube->player->rotat_angle) * indicator_length;
	end_y = cube->player->minimap_player_y + sin(cube->player->rotat_angle) * indicator_length;
	dx = abs(end_x - cube->player->minimap_player_x);
	dy = abs(end_y - cube->player->minimap_player_y);
	sx = cube->player->minimap_player_x < end_x ? 1 : -1;
	sy = cube->player->minimap_player_y < end_y ? 1 : -1;
	err = dx - dy;
	while ((int)cube->player->minimap_player_x != end_x || (int)cube->player->minimap_player_y != end_y)
	{
		mlx_put_pixel(cube->image, cube->player->minimap_player_x, cube->player->minimap_player_y, c_rgba(255, 0, 0, 255));
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			cube->player->minimap_player_x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			cube->player->minimap_player_y += sy;
		}
	}
}

void	ft_draw_map_background(t_cub *cube)
{
	int	y;
	int	x;
	int	sx;
	int	sy;
	int	dx;
	int	dy;

	y = -(cube->player->minimap_size / 2);
	while (y <= (cube->player->minimap_size / 2))
	{
		x = -(cube->player->minimap_size / 2);
		while (x <= (cube->player->minimap_size / 2))
		{
			sx = cube->player->minimap_start_x + (cube->player->minimap_size / 2) + x;
			sy = cube->player->minimap_start_y + (cube->player->minimap_size / 2) + y;
			dx = x;
			dy = y;
			if (cube->player->minimap == 1 && cube->player->tab != 1)
			{
				if (check_drow_minimap(cube, dx, dy, 2) == 1)
					mlx_put_pixel(cube->image, sx, sy, c_rgba(0, 0, 0, 1));
			}
			else
			{
				if (check_drow_minimap(cube, dx, dy, 1) == 1)
				{
					if (check_drow_minimap_1(cube, x, y, 1) == 1)
						mlx_put_pixel(cube->image, sx, sy, c_rgba(0, 0, 0, 255));
					else
						mlx_put_pixel(cube->image, sx, sy, c_rgba(0, 0, 0, 1));
				}
			}
			x++;
		}
		y++;
	}
}
