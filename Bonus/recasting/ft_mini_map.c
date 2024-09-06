#include "../../recasting.h"

int	check_drow_minimap(t_cub *cube, int dx, int dy, int is)
{
	if (is == 1)
	{
		if (dx >= -(cube->player->minimap_size / 2)
			&& dx <= (cube->player->minimap_size / 2)
			&& dy >= -(cube->player->minimap_size / 2)
			&& dy <= (cube->player->minimap_size / 2))
			return (1);
	}
	else if (is == 2)
	{
		if (dx * dx + dy * dy
			<= (cube->player->minimap_size / 2)
			* (cube->player->minimap_size / 2))
			return (1);
	}
	return (0);
}

int	check_drow_minimap_1(t_cub *cube, int x, int y, int is)
{
	if (is == 1)
	{
		if (y == -(cube->player->minimap_size / 2)
			|| x == -(cube->player->minimap_size / 2)
			|| x == (cube->player->minimap_size / 2)
			|| y == (cube->player->minimap_size / 2))
			return (1);
	}
	else if (is == 1)
	{
		if (y == -(cube->player->minimap_size / 2)
			|| x == -(cube->player->minimap_size / 2)
			|| x == (cube->player->minimap_size / 2)
			|| y == (cube->player->minimap_size / 2)
			|| y == -(cube->player->minimap_size / 2) + 1
			|| x == -(cube->player->minimap_size / 2) + 1
			|| x == (cube->player->minimap_size / 2) - 1
			|| y == (cube->player->minimap_size / 2) - 1)
			return (1);
	}
	return (0);
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

void	init_wh_if(t_cub *cube, int is, int x, int y)
{
	int	mm_st_x;
	int	mm_st_y;
	int	mm_size;

	mm_size = cube->player->minimap_size;
	mm_st_x = cube->player->minimap_start_x;
	mm_st_y = cube->player->minimap_start_y;
	if (is == 1)
	{
		cube->map_data.world_x = cube->player->player_x + x * 2;
		cube->map_data.world_y = cube->player->player_y + y * 2;
		cube->map_data.map_x = floor(cube->map_data.world_x / tile_size);
		cube->map_data.map_y = floor(cube->map_data.world_y / tile_size);
	}
	else
	{
		cube->map_data.color = c_rgba(192, 192, 192, 255);
		if (cube->data->map[cube->map_data.map_y][cube->map_data.map_x] == '1')
			cube->map_data.color = c_rgba(0, 0, 0, 100);
		cube->map_data.screen_x = mm_st_x + (mm_size / 2) + x;
		cube->map_data.screen_y = mm_st_y + (mm_size / 2) + y;
		cube->map_data.dx = x;
		cube->map_data.dy = y;
	}
}

void	check_put_pxl(t_cub *cube)
{
	int	a;
	int	b;

	b = (cube->player->minimap_size / 2) * (cube->player->minimap_size / 2);
	a = cube->map_data.dx * cube->map_data.dx + cube->map_data.dy
		* cube->map_data.dy;
	if (cube->player->minimap == 1 && cube->player->tab != 1)
	{
		if (a <= b)
			mlx_put_pixel(cube->image, cube->map_data.screen_x,
				cube->map_data.screen_y, cube->map_data.color);
	}
	else
	{
		if (cube->map_data.dx >= -(cube->player->minimap_size / 2)
			&& cube->map_data.dx <= (cube->player->minimap_size / 2)
			&& cube->map_data.dy >= -(cube->player->minimap_size / 2)
			&& cube->map_data.dy <= (cube->player->minimap_size / 2))
			mlx_put_pixel(cube->image, cube->map_data.screen_x,
				cube->map_data.screen_y, cube->map_data.color);
	}
}

void	ft_draw_minimap_to_screen(t_cub *cube)
{
	int	y;
	int	x;

	y = -(cube->player->minimap_size / 2);
	while (y <= (cube->player->minimap_size / 2))
	{
		x = -(cube->player->minimap_size / 2);
		while (x <= (cube->player->minimap_size / 2))
		{
			x++;
			init_wh_if(cube, 1, x, y);
			if (cube->map_data.map_x < cube->data->map_cols
				&& cube->map_data.map_x >= 0 && cube->map_data.map_y >= 0
				&& cube->map_data.map_y < cube->data->map_row)
			{
				init_wh_if(cube, 2, x, y);
				check_put_pxl(cube);
			}
		}
		y++;
	}
}

void	ft_draw_player(t_cub *cube)
{
	int	player_radius;
	int	j;
	int	i;

	player_radius = 2;
	j = -player_radius;
	while (j <= player_radius)
	{
		i = -player_radius;
		while (i <= player_radius)
		{
			if (i * i + j * j <= player_radius * player_radius)
				mlx_put_pixel(cube->image,
					cube->player->minimap_player_x + i * 2,
					cube->player->minimap_player_y + j * 2,
					c_rgba(255, 0, 0, 255));
			i++;
		}
		j++;
	}
}

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

void	ft_draw_border_1(t_cub *cube, int x, int y)
{
	int	sx;
	int	sy;
	int	dx;
	int	dy;
	int	mz;

	mz = cube->player->minimap_size;
	while (x <= (cube->player->minimap_size / 2))
	{
		sx = cube->player->minimap_start_x + (mz / 2) + x;
		sy = cube->player->minimap_start_y + (mz / 2) + y;
		dx = x;
		dy = y;
		if (cube->player->minimap != 1)
		{
			if (check_drow_minimap(cube, dx, dy, 1) == 1)
			{
				if (check_drow_minimap_1(cube, x, y, 2) == 1)
					mlx_put_pixel(cube->image, sx, sy, c_rgba(0, 0, 0, 255));
			}
		}
		x++;
	}
}

void	ft_draw_border(t_cub *cube)
{
	int	y;
	int	x;

	y = -(cube->player->minimap_size / 2);
	while (y <= (cube->player->minimap_size / 2))
	{
		x = -(cube->player->minimap_size / 2);
		ft_draw_border_1(cube, x, y);
		y++;
	}
}

void	draw_minimap(t_cub *cube)
{
	int	m_s;

	cube->player->minimap_start_y = 10;
	if (cube->player->minimap == 1 && cube->player->tab != 1)
	{
		cube->player->minimap_size = 200;
		cube->player->minimap_start_x = WIDTH / 1.12;
	}
	else
	{
		cube->player->minimap_size = 500;
		cube->player->minimap_start_x = WIDTH / 1.35;
	}
	m_s = cube->player->minimap_size;
	ft_draw_map_background(cube);
	ft_draw_minimap_to_screen(cube);
	cube->player->minimap_player_x = cube->player->minimap_start_x + (m_s / 2);
	cube->player->minimap_player_y = cube->player->minimap_start_y + (m_s / 2);
	ft_draw_player(cube);
	ft_draw_player_indecator(cube);
	ft_draw_border(cube);
}
//end draw_player

void	draw_per(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
			if (cube->data->map[y][x] == cube->data->p)
				draw_minimap(cube);
	}
}
