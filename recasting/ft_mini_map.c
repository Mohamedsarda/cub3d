#include "recasting.h"

int is_it_a_wall(double x, double y, t_cub *cube)
{
    double left = x - ((double)cube->player->radius  * cube->scale);
    double up = y + ((double)cube->player->radius  * cube->scale);
    double right = x + ((double)cube->player->radius  * cube->scale);
    double down = y - ((double)cube->player->radius  * cube->scale);

    if(left < 0 || right > cube->data->map_cols * (tile_size * cube->scale) || up < 0 || down > cube->data->map_row * (tile_size * cube->scale))
        return (0);

    int  t_left = floor(left / (tile_size * cube->scale));
    int  t_up = floor(up / (tile_size * cube->scale));
    int  t_right = floor(right / (tile_size * cube->scale));
    int  t_down = floor(down / (tile_size * cube->scale));

    if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
        || cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
        return (0);
    return (1);
}

void draw_view_player(t_cub *cube, int is)
{
    // Normalize the player's rotation angle
    if (is == 1)
        cube->scale = 0.2;
    else
        cube->scale = 1;
    cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
    cube->player->rotat_angle += (double)cube->player->turn_direction * cube->player->rotation_speed;

    // Calculate movement step
    double movestep = (double)cube->player->walk_direction * cube->player->move_speed;
    double new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
    double new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

    // Handle strafe movement (left/right)
    if (cube->player->strafe_direction != 0)
    {
        new_player_x += (double)cube->player->strafe_direction * (cube->player->move_speed / 2) * cos(cube->player->rotat_angle + M_PI / 2);
        new_player_y += (double)cube->player->strafe_direction * (cube->player->move_speed / 2) * sin(cube->player->rotat_angle + M_PI / 2);
    }

    // Check if the new position is within bounds and not a wall
    if (is_it_a_wall(new_player_x, new_player_y, cube))
    {
        cube->player->player_x = new_player_x;
        cube->player->player_y = new_player_y;
    }
    else
    {
        // Try sliding along the wall
        if (is_it_a_wall(cube->player->player_x, new_player_y, cube))
            cube->player->player_y = new_player_y;
        else if (is_it_a_wall(new_player_x, cube->player->player_y, cube))
            cube->player->player_x = new_player_x;
    }
    cube->scale = 1;
    if (is == 1)
    {
        // Draw the player and the 2D map view
        cube->scale = 0.2;
        draw_player(cube);
        draw_lines(cube);
    }
    else
        draw_lines_3D(cube);
}

void draw_player(t_cub *cube)
{
    int radius = cube->player->radius * cube->scale;
    double centerX = cube->player->player_x;
    double centerY = cube->player->player_y;

    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
            if (x * x + y * y <= radius * radius)
                my_mlx_pixel_put(&cube->img, centerX + x, centerY + y, BLUE);
    }
}

void	handle_pixel2(int x, int y, t_cub *cube, int is)
{
    if(cube->data->map[y][x] == cube->data->p)
    {
        draw_cube(cube, x, y, RED, is);
        draw_view_player(cube, is);
    }
}

void	draw_per(t_cub *cube, int is)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
			handle_pixel2(x, y, cube, is);
	}
}

// draw_map

void handle_pixel(int x, int y, t_cub *cube)
{
    // Compute the screen coordinates based on player's position
    if (cube->data->map[y][x] == '1')
        draw_cube(cube, x, y, WHITE, 1);
    else if (cube->data->map[y][x] == '0')
        draw_cube(cube, x, y, GRAY, 1);
}

void draw_map(t_cub *cube)
{
    int x, y;
    y = -1;
    while (++y < cube->data->map_row)
    {
        x = -1;
        while (++x < cube->data->map_cols)
            handle_pixel(x, y, cube);
    }
}

void    draw_cube(t_cub *cube, int x, int y, int color, int is)
{
	int	i;
	int	j;

	j = -1;
    if (is == 1)
    {
        while (++j < ((tile_size -1) * 0.2))
        {
            i = -1;
            while (++i < ((tile_size -1) * 0.2))
                my_mlx_pixel_put(&cube->img,  (x * ((tile_size) * 0.2)) + i ,  (y * ((tile_size) * 0.2)) + j, color);
        }
        return ;
    }
}
