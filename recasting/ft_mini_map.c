#include "recasting.h"


void ft_draw_map_background(t_cub *cube, int minimap_size, double xx, double yy)
{
	int	y;
	int	x;
	int	screen_x;
	int	screen_y;
	int	dx;
	int	dy;

	y = -(minimap_size / 2);
	while (y <= (minimap_size / 2))
	{
		x = -(minimap_size / 2);
		while (x <= (minimap_size / 2))
		{
			screen_x = xx + (minimap_size / 2) + x;
			screen_y = yy + (minimap_size / 2) + y;

			dx = x;
			dy = y;
			if (dx * dx + dy * dy <= (minimap_size / 2) * (minimap_size / 2))
				mlx_put_pixel(cube->image, screen_x, screen_y, create_rgba(192, 192, 192, 100));
			x++;
		}
		y++;
	}
}

void    ft_draw_minimap_to_screen(t_cub *cube, int minimap_size, int minimap_start_x, int minimap_start_y)
{
	int	y;
	int	x;

	y = -(minimap_size / 2);
	while (y <= (minimap_size / 2))
	{
		x = -(minimap_size / 2);
		while (x <= (minimap_size / 2))
		{
			x++;
			cube->map_data.world_x = cube->player->player_x + x * 2;
			cube->map_data.world_y = cube->player->player_y + y * 2;
			cube->map_data.map_x = floor(cube->map_data.world_x / tile_size);
			cube->map_data.map_y = floor(cube->map_data.world_y / tile_size);
			if (cube->map_data.map_x >= 0 && cube->map_data.map_x < cube->data->map_cols && cube->map_data.map_y >= 0 && cube->map_data.map_y < cube->data->map_row)
			{
				cube->map_data.color = create_rgba(192, 192, 192, 255);
				if (cube->data->map[cube->map_data.map_y][cube->map_data.map_x] == '1')
					cube->map_data.color = create_rgba(255, 255, 255, 255);
				cube->map_data.screen_x = minimap_start_x + (minimap_size / 2) + x;
				cube->map_data.screen_y = minimap_start_y + (minimap_size / 2) + y;
				cube->map_data.dx = x;
				cube->map_data.dy = y;
				if (cube->map_data.dx * cube->map_data.dx + cube->map_data.dy * cube->map_data.dy <= (minimap_size / 2) * (minimap_size / 2))
					mlx_put_pixel(cube->image, cube->map_data.screen_x, cube->map_data.screen_y, cube->map_data.color);
			}
		}
		y++;
	}
}

void    ft_draw_player(t_cub *cube, int minimap_player_x, int minimap_player_y)
{

	int	player_radius;
	int	j;
	int	i;

	player_radius = 2;
	j =  -player_radius;
	while (j <= player_radius)
	{
		i = -player_radius;
		while (i <= player_radius)
		{
			if (i * i + j * j <= player_radius * player_radius)
				mlx_put_pixel(cube->image, minimap_player_x + i * 2, minimap_player_y + j * 2, create_rgba(255, 0, 0, 255));
			i++;
		}
		j++;
	}
}

void    ft_draw_player_indecator(t_cub *cube, int minimap_player_x, int minimap_player_y)
{
	int indicator_length = 20;
	int end_x = minimap_player_x + cos(cube->player->rotat_angle) * indicator_length;
	int end_y = minimap_player_y + sin(cube->player->rotat_angle) * indicator_length;
	int dx = abs(end_x - minimap_player_x);
	int dy = abs(end_y - minimap_player_y);
	int sx = minimap_player_x < end_x ? 1 : -1;
	int sy = minimap_player_y < end_y ? 1 : -1;
	int err = dx - dy;

	while ((int)minimap_player_x != end_x || (int)minimap_player_y != end_y)
	{
		mlx_put_pixel(cube->image, minimap_player_x, minimap_player_y, create_rgba(255, 0, 0, 255));
		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			minimap_player_x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			minimap_player_y += sy;
		}
	}
}

void draw_minimap(t_cub *cube)
{
	int minimap_size = 200;

	int minimap_start_x = 10;
	int minimap_start_y = cube->image->height - minimap_size - 10;

	ft_draw_map_background(cube, minimap_size, minimap_start_x, minimap_start_y);
	ft_draw_minimap_to_screen(cube, minimap_size, minimap_start_x, minimap_start_y);
	int minimap_player_x = minimap_start_x + (minimap_size / 2);
	int minimap_player_y = minimap_start_y + (minimap_size / 2);
	ft_draw_player(cube, minimap_player_x, minimap_player_y);
	ft_draw_player_indecator(cube, minimap_player_x, minimap_player_y);
}
//end draw_player


void	handle_pixel2(int x, int y, t_cub *cube)
{
	if(cube->data->map[y][x] == cube->data->p)
		draw_minimap(cube);
}

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
			handle_pixel2(x, y, cube);
	}
}

