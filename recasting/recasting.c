#include "recasting.h"

void	malloc_error(void)
{
	printf("error in malloc\n");
	exit(EXIT_FAILURE);
}

double deg2rad(double degrees)
{
    return degrees * (M_PI / 180.0);
}

t_player    *init_player(t_cub *cube)
{
    int	x;
	int	y;
    t_player *player;

	y = -1;
    player = (t_player *)malloc(sizeof(t_player));
    if (!player)
        malloc_error();
	while (++y < map_row)
	{
		x = -1;
		while (++x < map_cols)
        {
            if(cube->map[y][x] == 'N')
            {
                player->player_x = x * tile_size + (tile_size / 2);
                player->player_y = y * tile_size + (tile_size / 2);
            }
        }
	}
    player->radius = 10;
    player->move_speed = 2.0;
    player->rotat_angle = deg2rad(270); 
    player->rotation_speed = 2 * (M_PI / 180);
    player->turn_direction = 0;
    player->walk_direction = 0;

    return (player);
}

void    ft_fractol_init(t_cub *cube, char **map)
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
    cube->map = map;
    cube->player = init_player(cube);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;
	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

void    draw_cube(t_cub *cube, int x, int y, int color)
{
	int	i;
	int	j;

	j = -1;
    while (++j < tile_size - 1)
    {
        i = -1;
        while (++i < tile_size - 1)
            my_mlx_pixel_put(&cube->img, (x * tile_size) + i , (y * tile_size) + j, color);
    }
}

// draw_player
int is_it_a_wall(double x, double y, t_cub *cube)
{
    double left = x - (cube->player->radius);
    double up = y + (cube->player->radius);
    double right = x + (cube->player->radius);
    double down = y - (cube->player->radius);
    
    if(left < 0 || right > WIDTH || up < 0 || down > HEIGHT)
        return (0);

    int  t_left = floor(left / tile_size);
    int  t_up = floor(up / tile_size);
    int  t_right = floor(right / tile_size);
    int  t_down = floor(down / tile_size);

    if (cube->map[t_up][t_left] == '1' || cube->map[t_down][t_right] == '1'
        || cube->map[t_up][t_right] == '1' || cube->map[t_down][t_left] == '1')
        return (0);
    return (1);
}

void    draw_player(t_cub *cube, double circle_center_x, double circle_center_y)
{
    int x, y;
    y = -cube->player->radius;
    while (y <= cube->player->radius)
    {
        x = -cube->player->radius;
        while(x <= cube->player->radius)
        {
            if (x * x + y * y <= cube->player->radius * cube->player->radius)
                my_mlx_pixel_put(&cube->img, circle_center_x + x, circle_center_y + y, BLUE);
            x++;
        }
        y++;
    }
}

void DDA(t_cub *cube, int X0, int Y0, int X1, int Y1)
{ 
    // calculate dx & dy 
    int dx = X1 - X0; 
    int dy = Y1 - Y0; 
  
    // calculate steps required for generating pixels 
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy); 
  
    // calculate increment in x & y for each steps 
    float Xinc = dx / (float)steps; 
    float Yinc = dy / (float)steps; 
  
    // Put pixel for each step 
    float X = X0; 
    float Y = Y0; 
    for (int i = 0; i <= steps; i++) {
        my_mlx_pixel_put(&cube->img, X, Y, GREEN);
        X += Xinc; // increment in x at each step 
        Y += Yinc; // increment in y at each step 
    } 
} 

int has_wall(t_cub *cube, double x1, double y1)
{
    int x = floor(x1 / tile_size);
    int y = floor(y1 / tile_size);

    if (cube->map[y][x] == '1')
        return (1);
    return (0);
}

double normalizeAngle(double angle) {
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0) {
        angle += 2 * M_PI;
    }
    return angle;
}

// void draw_line(t_cub *cube, int circle_center_x, int circle_center_y, double j)
// {

//     double israyfacing_down = 0;
//     if(cube->player->rotat_angle > 0 && cube->player->rotat_angle < M_PI)
//         israyfacing_down = 1;
//     double israyfacing_up = !israyfacing_down;
//     double israyfacing_right = 0;
//     if(cube->player->rotat_angle <0.5 * M_PI  || cube->player->rotat_angle > 1.5 * M_PI)
//         israyfacing_right = 1;
//     double israyfacing_left = !israyfacing_right;

//     int new_line_end_x;
//     int new_line_end_y;
    
// // first cube x_y
//     double y_intercept = floor(circle_center_y/ tile_size) * tile_size;
//     if(israyfacing_down == 1)
//         y_intercept += tile_size;
//     double x_intercept = circle_center_x + ((circle_center_y - y_intercept) / tan(cube->player->rotat_angle));
//     // printf("---- y_intercept = %f | x_intercept = %f\n", y_intercept , x_intercept);
//     // printf("---- y_player = %d | x_player = %d\n", circle_center_y , circle_center_x);
// // end first cube x_y

// // steps
//     double ystep = tile_size;
//     if(israyfacing_up == 1)
//         ystep *= -1;
//     else
//         ystep *= 1;
//     double xstep = tile_size / tan(cube->player->rotat_angle);
//     if(israyfacing_left == 1 && xstep > 0)
//         xstep *= -1;
//     else
//         xstep *= 1;
//     if(israyfacing_right == 1 && xstep < 0)
//         xstep *= -1;
//     else
//         xstep *= 1;
//     // printf("++++ ystep = %f | xstep =  %f\n", ystep , xstep);
// // done steps

//     // double nexthorz_x = x_intercept;
//     // double nexthorz_y = y_intercept;
// // 
//     // if(israyfacing_up == 1)
//     //     nexthorz_y--;
//     // while (/* condition */)
//     // {
//     //     if(has_wall(cube, nexthorz_x, nexthorz_y))
//     //     {
// // 
//     //     }
//     //     else
//     //     {
//     //         nexthorz_x += xstep;
//     //         nexthorz_y += ystep;
//     //     }
// // 
//     // }
    
//     // if(israyfacing_up == 1)
//     //     y_intercept--;
//     // double start_x =  x_intercept;
//     // double start_y =  y_intercept;

//     // printf("%d, %d\n", a, b);

// // up down
//     // while (start_x >= 0 && start_x <= WIDTH && start_y >= 0 && start_y <= HEIGHT)
//     // {
//     //     int a = floor(start_x / tile_size);
//     //     int b = floor(start_y / tile_size);
//     //     // printf("%d, %d\n", a, b);
//     //     if(cube->map[b][a] == '1')
//     //     {
//     //         // puts("++++");
//     //         break;
//     //     }
//     //     start_y += ystep;
//     //     start_x += xstep;
//     //     printf("[%f] | [%f]\n+++++ \n", start_y, start_x);
//     //     // b -= 1;
//     // }
//     // // start_y -= ystep;
//     // // double start_y = tile_size;    
//     // if(israyfacing_up == 1)
//     //     start_y =  circle_center_y - start_y;
//     // else if(israyfacing_down == 1)
//     //     start_y =  start_y - circle_center_y;
//     // else if(israyfacing_left == 1)
//     //     start_y =  circle_center_x - start_x;
//     // else if(israyfacing_right == 1)
//     //     start_y =  start_x - circle_center_x;
//     // new_line_end_x = (circle_center_x  + (start_x * cos(j)));
//     // new_line_end_y = (circle_center_y  + ((start_y) * sin(j)));
//     // new_line_end_x = (circle_center_x  + (start_x * cos(j)));
//     // new_line_end_y = (circle_center_y  + ((start_y - circle_center_y) * sin(j)));
// //
  
//     // double y_intercept = 150;
//     // double x_intercept = 525;
//     // double ystep =  50;
//     // double xstep =  0;
//     // double start_x =  x_intercept = 525;
//     // double start_y =  y_intercept = 150;
//     // start_x = x_intercept / tile_size;
//     // start_y = y_intercept / tile_size;

//     // int f_x;
//     // int f_y;
//     // while (1)
//     // {
//     //     f_x = start_x / tile_size;
//     //     f_y = start_y / tile_size;
//     //     if(cube->map[f_y][f_x] == 1)
//     //     {
//     //         printf("f-y = %d | f-x = %d\n", f_y, f_x);
//     //         printf("start_y = %f | start_x = %f\n", start_y, start_x);
//     //         break;
//     //     }
//     //     start_x += xstep;
//     //     start_y -= ystep;
//     // }
//     // f_x -= xstep;
//     // f_y -= ystep;
//     // f_x *= tile_size;
//     // f_y *= tile_size;

//     // printf("[%d] | [%f]\n", circle_center_y, y_intercept);
//     new_line_end_x = (circle_center_x  + (x_intercept * cos(j)));
//     new_line_end_y = (circle_center_y  + ((circle_center_y - y_intercept) * sin(j)));

//     DDA(cube, circle_center_x, circle_center_y, new_line_end_x, new_line_end_y);
// }

double distanceBetweenPoints(x1, y1, x2, y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void draw_line(t_cub *cube, int circle_center_x, int circle_center_y, double angle)
{
    (void)angle;
    cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);

    int isRayFacingDown = cube->player->rotat_angle > 0 && cube->player->rotat_angle < M_PI;
    int isRayFacingUp = !isRayFacingDown;
    int isRayFacingRight = cube->player->rotat_angle < 0.5 * M_PI || cube->player->rotat_angle > 1.5 * M_PI;
    int isRayFacingLeft = !isRayFacingRight;

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    int foundHorzWallHit = 0;
    double horzWallHitX = 0;
    double horzWallHitY = 0;

    double y_intercept = floor(circle_center_y / tile_size) * tile_size;
    y_intercept += isRayFacingDown ? tile_size : 0;

    double x_intercept = circle_center_x + (y_intercept - circle_center_y) / tan(cube->player->rotat_angle);

    double ystep = tile_size;
    ystep *= isRayFacingUp ? -1 : 1;

    double xstep = tile_size / tan(cube->player->rotat_angle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    double nextHorzTouchX = x_intercept;
    double nextHorzTouchY = y_intercept;

    if (isRayFacingUp)
        nextHorzTouchY--;

    while (nextHorzTouchX >= 0 && nextHorzTouchX <= WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= HEIGHT) {
        if (has_wall(cube, nextHorzTouchX, nextHorzTouchY)) {
            foundHorzWallHit = 1;
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            break;
        } else
        {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }

    ///////////////////////////////////////////
    // VERTICAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    int foundVertWallHit = 0;
    double vertWallHitX = 0;
    double vertWallHitY = 0;

    x_intercept = floor(circle_center_x / tile_size) * tile_size;
    x_intercept += isRayFacingRight ? tile_size : 0;

    y_intercept = circle_center_y + (x_intercept - circle_center_x) * tan(cube->player->rotat_angle);

    xstep = tile_size;
    xstep *= isRayFacingLeft ? -1 : 1;

    ystep = tile_size * tan(cube->player->rotat_angle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    double nextVertTouchX = x_intercept;
    double nextVertTouchY = y_intercept;

    if (isRayFacingLeft)
        nextVertTouchX--;

    while (nextVertTouchX >= 0 && nextVertTouchX <= WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= HEIGHT) {
        if (has_wall(cube, nextVertTouchX, nextVertTouchY)) {
            foundVertWallHit = 1;
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            break;
        } else {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical distances and choose the smallest value
    double horzHitDistance = (foundHorzWallHit)
        ? distanceBetweenPoints(circle_center_x, circle_center_y, horzWallHitX, horzWallHitY)
        : __DBL_MAX__;
    double vertHitDistance = (foundVertWallHit)
        ? distanceBetweenPoints(circle_center_x, circle_center_y, vertWallHitX, vertWallHitY)
        : __DBL_MAX__;

    // Only store the smallest of the distances
    double wallHitX = (horzHitDistance < vertHitDistance) ? horzWallHitX : vertWallHitX;
    double wallHitY = (horzHitDistance < vertHitDistance) ? horzWallHitY : vertWallHitY;

    DDA(cube, circle_center_x, circle_center_y, wallHitX, wallHitY);
}
void    draw_lines(t_cub *cube, int circle_center_x, int circle_center_y)
{
    double angle;

    angle = cube->player->rotat_angle + FOV_ANGLE / 2;
    int i = 0;
    while (i < NUM_RAYS)
    {
        draw_line(cube, circle_center_x, circle_center_y, angle);
        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
    
}

void draw_view_player(t_cub *cube)
{
    // get new_angle of view and if he move or not
    int movestep = cube->player->walk_direction * cube->player->move_speed;

    // new_position of the player
    double new_player_x = 0.0;
    double new_player_y = 0.0;
    new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
    new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

    // check new_position is in a wall or not
    if(is_it_a_wall(new_player_x, new_player_y, cube) == 1)
    {
        // if (cube->player->walk_direction != 0)
        // {
        //     cube->player->player_x += movestep * cos(cube->player->rotat_angle);
        //     cube->player->player_y += movestep * sin(cube->player->rotat_angle);
        //     // cube->player->player_x += cube->player->walk_direction * cube->player->move_speed * cos(cube->player->rotat_angle);
        //     // cube->player->player_y += cube->player->walk_direction * cube->player->move_speed * sin(cube->player->rotat_angle);
        // }
        cube->player->player_x = new_player_x;
        cube->player->player_y = new_player_y;
    }
    
    draw_player(cube, cube->player->player_x, cube->player->player_y);
    draw_line(cube, cube->player->player_x, cube->player->player_y, cube->player->rotat_angle);
    // draw_lines(cube, cube->player->player_x, cube->player->player_y);
}

void	handle_pixel2(int x, int y, t_cub *cube)
{
    if(cube->map[y][x] == 'N')
    {
        draw_cube(cube, x, y, RED);
        draw_view_player(cube);
    }
}

void	draw_per(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < map_row)
	{
		x = -1;
		while (++x < map_cols)
			handle_pixel2(x, y, cube);
	}
}
//end draw_player

// draw_map
void	handle_pixel(int x, int y, t_cub *cube, char **map)
{
    if(map[y][x] == '1')
        draw_cube(cube, x, y, WHITE);
    else if(map[y][x] == '0')
        draw_cube(cube, x, y, RED);
}

void	draw_map(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < map_row)
	{
		x = -1;
		while (++x < map_cols)
			handle_pixel(x, y, cube, cube->map);
	}
}
// end draw_map


// all_black
void	handle_pixel3(int x, int y, t_cub *cube)
{
    
	int	i;
	int	j;
	j = -1;
    while (++j < tile_size - 1)
    {
        i = -1;
        while (++i < tile_size - 1)
            my_mlx_pixel_put(&cube->img, (x * tile_size) + i , (y * tile_size) + j, BLACK);
    }
}

void	draw_all_black(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < map_row)
	{
		x = -1;
		while (++x < map_cols)
			handle_pixel3(x, y, cube);
	}
}
// end all_black

// hooks
int	handle_close_button(t_cub *data)
{
	mlx_destroy_window(data->mlx_con, data->mlx_win);
	exit(1);
}

int handle_input_key_down(int keycode, t_cub * data)
{
    if(keycode == 124)
        data->player->turn_direction = 1;
    else if(keycode == 125)
        data->player->walk_direction = -1;
    else if(keycode == 13)
        mlx_clear_window(data->mlx_con, data->mlx_win);
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
	return (0);
}

int handle_input_key_up(int keycode, t_cub * data)
{

    if (keycode == 124 || keycode == 123) 
        data->player->turn_direction = 0;
    else if (keycode == 125 || keycode == 126)
        data->player->walk_direction = 0;
	return (0);
}

void    update_player(t_cub * cube)
{
    cube->player->rotat_angle += cube->player->turn_direction * cube->player->rotation_speed;
    
}

int loop_fun(t_cub * cube)
{
    draw_all_black(cube);
    update_player(cube);
    draw_map(cube);
    draw_per(cube);
    // mlx_clear_window(cube->mlx_con, cube->mlx_win);
	mlx_put_image_to_window(cube->mlx_con, cube->mlx_win, cube->img.img_ptr, 0, 0);
    return (0);
}
// end hooks