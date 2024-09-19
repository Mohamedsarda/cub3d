#include "../../../recasting.h"

static void	init_wh_if(t_cub *cube, int is, int x, int y)
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
		cube->map_data.map_x = floor(cube->map_data.world_x / TILE_SIZE);
		cube->map_data.map_y = floor(cube->map_data.world_y / TILE_SIZE);
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

static void	check_put_pxl(t_cub *cube)
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
