#include "recasting.h"
double deg2rad(double degrees)
{
    return degrees * (M_PI / 180.0);
}

double rad2deg(double radians)
{
    return radians * (180.0 / M_PI);
}

void	malloc_error(void)
{
	printf("error in malloc\n");
	exit(EXIT_FAILURE);
}

t_player    *init_player(void)
{
    t_player *player;
    player = (t_player *)malloc(sizeof(t_player));
    if (!player)
        malloc_error();
    player->player_x = 0.0;
    player->player_y = 0.0;
    player->radius = 5;
    player->move_speed = 2.0;
    player->rotat_angle = M_PI / 3;
    player->rotation_speed = 2 * (M_PI / 180);
    player->turn_direction = 0;
    player->walk_direction = 0;

    return (player);
}


void    ft_fractol_init(t_cub *cube)
{
    cube->mlx_con = mlx_init();
	if(!cube->mlx_con)
		malloc_error();
	cube->mlx_win = mlx_new_window(cube->mlx_con, WIDTH, HEIGHT, "Cub3D");
	if(!cube->mlx_win)
		malloc_error();
	cube->img.img_ptr = mlx_new_image(cube->mlx_con, WIDTH, HEIGHT);
	if(!cube->img.img_ptr)
	{
		mlx_destroy_window(cube->mlx_con, cube->mlx_win);
		malloc_error();
	}
	cube->img.pixels_ptr = mlx_get_data_addr(cube->img.img_ptr, &cube->img.bpp, &cube->img.line_len, &cube->img.endian);
	if(!cube->img.pixels_ptr)
	{
		mlx_destroy_image(cube->mlx_con, cube->img.img_ptr);
		mlx_destroy_window(cube->mlx_con, cube->mlx_win);
		malloc_error();
	}
    cube->player = init_player();
}

int	handle_close_button(t_cub *data)
{
	mlx_destroy_window(data->mlx_con, data->mlx_win);
	exit(1);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;
	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}


void draw_filled_circle(t_cub *cube, int color)
{
    int x, y;
    // int movestep = cube->player->walk_direction * cube->player->move_speed;
    // cube->player->player_x += cos(cube->player->rotat_angle) * cube->player->walk_direction * cube->player->move_speed;
    // cube->player->player_y += sin(cube->player->rotat_angle) * cube->player->walk_direction * cube->player->move_speed;
     cube->player->rotat_angle += (cube->player->turn_direction * cube->player->rotation_speed);

    // Update player's position based on movement direction and rotation angle
    if (cube->player->walk_direction != 0)
    {
        double delta_x = cos(cube->player->rotat_angle) * cube->player->walk_direction * cube->player->move_speed;
        double delta_y = sin(cube->player->rotat_angle) * cube->player->walk_direction * cube->player->move_speed;
        cube->player->player_x += delta_x;
        cube->player->player_y += delta_y;

        // Debugging output
        printf("delta_x: %f | delta_y: %f | player_x: %f | player_y: %f | walk_direction: %d | move_speed: %f | cos(rotat_angle): %f\n",
               delta_x, delta_y, cube->player->player_x, cube->player->player_y, cube->player->walk_direction, cube->player->move_speed, cos(cube->player->rotat_angle));
    }
    // printf("[%d] | [%d]\n", cube->player->player_x, cube->player->player_y);
    int circle_center_x = cube->player->player_x + (tile_size / 2);
    int circle_center_y = cube->player->player_y + (tile_size / 2);
    for (y = -cube->player->radius; y <= cube->player->radius; y++)
    {
        for (x = -cube->player->radius; x <= cube->player->radius; x++)
        {
            if (x * x + y * y <= cube->player->radius * cube->player->radius)
                my_mlx_pixel_put(&cube->img, circle_center_x + x, circle_center_y + y, color);
        }
    }

    // printf("+++++[%d]\n",cube->player->turn_direction);
    // cube->player->rotat_angle += (cube->player->turn_direction * cube->player->rotation_speed);
    double angle_rad = cube->player->rotat_angle;
    int line_length = 50; // Length of the line

    // Calculate endpoint coordinates of the line
    int line_end_x = circle_center_x + line_length * cos(angle_rad);
    int line_end_y = circle_center_y + line_length * sin(angle_rad);

    // Draw the line using Bresenham's line algorithm or directly plotting pixels
    // Example using Bresenham's line algorithm
    int dx = abs(line_end_x - circle_center_x);
    int sx = circle_center_x < line_end_x ? 1 : -1;
    int dy = -abs(line_end_y - circle_center_y);
    int sy = circle_center_y < line_end_y ? 1 : -1;
    int err = dx + dy;
    while (1)
    {
        my_mlx_pixel_put(&cube->img, circle_center_x, circle_center_y, color); // Draw pixel
        if (circle_center_x == line_end_x && circle_center_y == line_end_y)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            circle_center_x += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            circle_center_y += sy;
        }
    }
}

void	handle_pixel(int x, int y, t_cub *cube, int map[map_row][map_cols])
{
    
	int	i;
	int	j;
    t_player *player;
	j = 0;
	i = 0;
    if(map[y][x] == 1)
    {
        while (j < tile_size - 1)
        {
            i = 0;
            while (i < tile_size - 1)
            {
                my_mlx_pixel_put(&cube->img, (x * tile_size) + i , (y * tile_size) + j, WHITE);
                i++;
            }
            j++;
        }
        return ;
    }
    if(map[y][x] == 0)
    {
        while (j < tile_size -1)
        {
            i = 0;
            while (i < tile_size -1)
            {
                my_mlx_pixel_put(&cube->img, (x * tile_size) + i , (y * tile_size) + j, RED);
                i++;
            }
            j++;
        }
        return ;
    }
    if(map[y][x] == 2)
    {
        cube->player->player_x = x * tile_size;
        cube->player->player_y = y * tile_size;
        while (j < tile_size -1)
        {
            i = 0;
            while (i < tile_size -1)
            {
                my_mlx_pixel_put(&cube->img, (x * tile_size) + i , (y * tile_size) + j, RED);
                i++;
            }
            j++;
        }
        draw_filled_circle(cube, BLUE);
        return ;
    }
}

void	handle_pixel2(int x, int y, t_cub *cube, int map[map_row][map_cols])
{
    
	int	i;
	int	j;
    t_player *player;
	j = 0;
	i = 0;
    if(map[y][x] == 2)
    {
        cube->player->player_x = x * tile_size;
        cube->player->player_y = y * tile_size;
        while (j < tile_size -1)
        {
            i = 0;
            while (i < tile_size -1)
            {
                my_mlx_pixel_put(&cube->img, (x * tile_size) + i , (y * tile_size) + j, RED);
                i++;
            }
            j++;
        }
        draw_filled_circle(cube, BLUE);
        return ;
    }
}

void	get_all_done(t_cub *cube)
{
	int	x;
	int	y;
int map[map_row][map_cols] = {  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                                    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
                                    };
	y = 0;
	x = 0;
	while (y < map_row)
	{
		x = 0;
		while (x < map_cols)
        {
			handle_pixel(x, y, cube, map);
            x++;
        }
        y++;
	}
    x = 0;
    y = 0;
	while (y < map_row)
	{
		x = 0;
		while (x < map_cols)
        {
			handle_pixel2(x, y, cube, map);
            x++;
        }
        y++;
	}
	mlx_put_image_to_window(cube->mlx_con, cube->mlx_win, cube->img.img_ptr, 0, 0);
}

// else if(keycode == 264)
// {
//     data->player->walk_up_down -= 3;
//     data->player->walk_right_left += 3;
// }
// else if(keycode == 269)
// {
//     data->player->walk_up_down -= 3;
//     data->player->walk_right_left -= 3;
// }
int handle_input_key_down(int keycode, t_cub * data)
{
    // printf("%d\n", keycode);
    if(keycode == 124)
        data->player->turn_direction = +1;
    else if(keycode == 125)
        data->player->walk_direction = -1;
    else if(keycode == 123)
        data->player->turn_direction = -1;
    else if(keycode == 126)
        data->player->walk_direction = 1;
    if (keycode == 53)
	{
		mlx_destroy_window(data->mlx_con, data->mlx_win);
		free(data->mlx_con);
		exit(1);
	}
    get_all_done(data);
	return (0);
}

int handle_input_key_up(int keycode, t_cub * data)
{
    // printf("%d\n", keycode);
    if (keycode == 124 || keycode == 123) 
        data->player->turn_direction = 0;
    else if (keycode == 125 || keycode == 126)
        data->player->walk_direction = 0;
    get_all_done(data);
	return (0);
}

int main()
{
    // const double fov_angle = 60 * M_PI / 180.0;
    // const double nmbr_rays = 320;
    // double rayangle = rayangle + (fov_angle / nmbr_rays);
    t_cub cube;

    ft_fractol_init(&cube);

    get_all_done(&cube);
    mlx_hook(cube.mlx_win, 17, 1L << 17, handle_close_button, &cube);
    mlx_hook(cube.mlx_win, 2, 0L, handle_input_key_down, &cube);
    mlx_hook(cube.mlx_win, 3, 0L, handle_input_key_up, &cube);
	mlx_loop(cube.mlx_con);

}