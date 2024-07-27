#include "recasting.h"
#include <sys/time.h>

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
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
        {
            if(cube->data->map[y][x] == 'N')
            {
                player->player_x = x * (tile_size * MAP_SCALE) + ((tile_size * MAP_SCALE) / 2);
                player->player_y = y * (tile_size * MAP_SCALE) + ((tile_size * MAP_SCALE) / 2);
            }
        }
	}
    player->radius = 10;
    player->move_speed = 1.5;
    player->rotat_angle = deg2rad(90);
    player->rotation_speed = 0.05;
    player->turn_direction = 0;
    player->walk_direction = 0;

    return (player);
}

void    ft_fractol_init(t_cub *cube)
{

    cube->mlx_con = mlx_init();
	if(!cube->mlx_con)
		malloc_error();
	cube->mlx_win = mlx_new_window(cube->mlx_con, cube->data->map_cols * tile_size, cube->data->map_row * tile_size, "Cub3D");
	if(!cube->mlx_win)
		malloc_error();
	cube->img.img_ptr = mlx_new_image(cube->mlx_con, cube->data->map_cols * tile_size, cube->data->map_row * tile_size);
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
    cube->player = init_player(cube);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;
	offset = (y * img->line_len) + (x * (img->bpp / 8));
	*(unsigned int *)(img->pixels_ptr + offset) = color;
}

void    draw_cube(t_cub *cube, int x, int y, int color, int is)
{
	int	i;
	int	j;

	j = -1;
    if (is == 1)
    {
        while (++j < ((tile_size -1) * MAP_SCALE))
        {
            i = -1;
            while (++i < ((tile_size -1) * MAP_SCALE))
                my_mlx_pixel_put(&cube->img,  (x * ((tile_size) * MAP_SCALE)) + i ,  (y * ((tile_size) * MAP_SCALE)) + j, color);
        }
        return ;
    }
}

// draw_player
int is_it_a_wall(double x, double y, t_cub *cube)
{
    double left = x - ((double)cube->player->radius  * MAP_SCALE);
    double up = y + ((double)cube->player->radius  * MAP_SCALE);
    double right = x + ((double)cube->player->radius  * MAP_SCALE);
    double down = y - ((double)cube->player->radius  * MAP_SCALE);

    if(left < 0 || right > cube->data->map_cols * (tile_size * MAP_SCALE) || up < 0 || down > cube->data->map_row * (tile_size * MAP_SCALE))
        return (0);

    int  t_left = (int)floor(left / (tile_size * MAP_SCALE));
    int  t_up = (int)floor(up / (tile_size * MAP_SCALE));
    int  t_right = (int)floor(right / (tile_size * MAP_SCALE));
    int  t_down = (int)floor(down / (tile_size * MAP_SCALE));

    if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
        || cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
        return (0);
    return (1);
}

void draw_player(t_cub *cube)
{
    int radius = cube->player->radius * MAP_SCALE;
    int centerX = (int)cube->player->player_x;
    int centerY = (int)cube->player->player_y;

    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
            if (x * x + y * y <= radius * radius)
                my_mlx_pixel_put(&cube->img, centerX + x, centerY + y, BLUE);
    }
}

// void    draw_player(t_cub *cube, double circle_center_x, double circle_center_y)
// {
//     int x, y;
//     y = -cube->player->radius;
//     while (y <= cube->player->radius)
//     {
//         x = -cube->player->radius;
//         while(x <= cube->player->radius)
//         {
//             if (x * x + y * y <= cube->player->radius * cube->player->radius)
//                 my_mlx_pixel_put(&cube->img, circle_center_x + x, circle_center_y + y, BLUE);
//             x++;
//         }
//         y++;
//     }
// }

void DDA(t_cub *cube, double X0, double Y0, double X1, double Y1)
{
    // calculate dx & dy
    double dx = X1 - X0;
    double dy = Y1 - Y0;

    // calculate steps requiBLACK for generating pixels
    double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);

    // calculate increment in x & y for each steps
    double Xinc = dx / steps;
    double Yinc = dy / steps;

    // Put pixel for each step
    double X = X0;
    double Y = Y0;
    for (int i = 0; i <= steps; i++) {
        my_mlx_pixel_put(&cube->img, X, Y, GREEN);
        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
    }
}

int has_wall(t_cub *cube, t_vars *vars, int is)
{
    double x1 = 0;
    double y1 = 0;
    if(is == 0)
    {
        x1 = vars->nextHorzTouchX;
        y1 = vars->nextHorzTouchY;
        if (vars->isRayFacingUp && y1 > 0)
            y1--;
    }
    if(is == 1)
    {
        x1 = vars->nextVertTouchX;
        if (vars->isRayFacingLeft && x1 > 0)
            x1--;
        y1 = vars->nextVertTouchY;
    }
    int x = floor(x1 / tile_size);
    int y = floor(y1 / tile_size);

    if (cube->data->map[y][x] == '1')
        return (1);
    return (0);
}

double normalizeAngle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle;
}

double distanceBetweenPoints(double x1,double y1,double x2,double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void    ft_draw_hero(t_cub *cube, t_vars *vars, double circle_center_x, double circle_center_y)
{
    vars->y_intercept = floor(circle_center_y / (tile_size * MAP_SCALE)) * (tile_size * MAP_SCALE);
    if (vars->isRayFacingDown)
        vars->y_intercept += (double)(tile_size * MAP_SCALE);

    vars->x_intercept = circle_center_x + (vars->y_intercept - circle_center_y) / tan(vars->angle);

    vars->ystep = (double)(tile_size * MAP_SCALE);
    if (vars->isRayFacingUp)
        vars->ystep *= -1;

    vars->xstep = (double)(tile_size * MAP_SCALE) / tan(vars->angle);
    if (vars->isRayFacingLeft && vars->xstep > 0)
        vars->xstep *= -1;
    if (vars->isRayFacingRight && vars->xstep < 0)
        vars->xstep *= -1;

    vars->nextHorzTouchX = vars->x_intercept;
    vars->nextHorzTouchY = vars->y_intercept;


    while (vars->nextHorzTouchX >= 0 && vars->nextHorzTouchX < cube->data->map_cols * (tile_size * MAP_SCALE)
            && vars->nextHorzTouchY >= 0 && vars->nextHorzTouchY < cube->data->map_row * (tile_size * MAP_SCALE)) {
        if (has_wall(cube, vars, 0)) {
            vars->foundHorzWallHit = 1;
            vars->horzWallHitX = vars->nextHorzTouchX;
            vars->horzWallHitY = vars->nextHorzTouchY;
            break;
        } else
        {
            vars->nextHorzTouchX += vars->xstep;
            vars->nextHorzTouchY += vars->ystep;
        }
    }
}

void    ft_draw_ver(t_cub *cube, t_vars *vars, double circle_center_x, double circle_center_y)
{
    vars->foundVertWallHit = 0;
    vars->vertWallHitX = 0;
    vars->vertWallHitY = 0;

    vars->x_intercept = floor(circle_center_x / (double)(tile_size * MAP_SCALE)) * (tile_size * MAP_SCALE);
    if (vars->isRayFacingRight)
        vars->x_intercept += (double)(tile_size * MAP_SCALE);

    vars->y_intercept = circle_center_y + (vars->x_intercept - circle_center_x) * tan(vars->angle);

    vars->xstep = (double)(tile_size * MAP_SCALE);
    if (vars->isRayFacingLeft)
        vars->xstep *= -1;

    vars->ystep = (double)(tile_size * MAP_SCALE) * tan(vars->angle);
    if (vars->isRayFacingUp && vars->ystep > 0)
        vars->ystep *= -1;
    if (vars->isRayFacingDown && vars->ystep < 0)
        vars->ystep *= -1;

    vars->nextVertTouchX = vars->x_intercept;
    vars->nextVertTouchY = vars->y_intercept;


    while (vars->nextVertTouchX >= 0 && vars->nextVertTouchX < cube->data->map_cols * (tile_size * MAP_SCALE)
            && vars->nextVertTouchY >= 0 && vars->nextVertTouchY < cube->data->map_row * (tile_size * MAP_SCALE)) {
        if (has_wall(cube, vars, 1)) {
            vars->foundVertWallHit = 1;
            vars->vertWallHitX = vars->nextVertTouchX;
            vars->vertWallHitY = vars->nextVertTouchY;
            break;
        } else {
            vars->nextVertTouchX += vars->xstep;
            vars->nextVertTouchY += vars->ystep;
        }
    }
}


t_vars draw_line(t_cub *cube, double circle_center_x, double circle_center_y, double angle, int is)
{
    t_vars vars;
    vars.angle = normalizeAngle(angle);
    vars.isRayFacingDown = vars.angle > 0 && vars.angle < M_PI;
    vars.isRayFacingUp = !vars.isRayFacingDown;
    vars.isRayFacingRight = vars.angle < 0.5 * M_PI || vars.angle > 1.5 * M_PI;
    vars.isRayFacingLeft = !vars.isRayFacingRight;

    vars.foundHorzWallHit = 0;
    vars.horzWallHitX = 0;
    vars.horzWallHitY = 0;

    ft_draw_hero(cube, &vars, circle_center_x, circle_center_y);
    ft_draw_ver(cube, &vars, circle_center_x, circle_center_y);
    vars.horzHitDistance = INT_MAX;
    vars.vertHitDistance = INT_MAX;
    if (vars.foundHorzWallHit)
        vars.horzHitDistance = distanceBetweenPoints(circle_center_x, circle_center_y, vars.horzWallHitX, vars.horzWallHitY);
    if (vars.foundVertWallHit)
        vars.vertHitDistance = distanceBetweenPoints(circle_center_x, circle_center_y, vars.vertWallHitX, vars.vertWallHitY);
    // Only store the smallest of the distances
    vars.wallHitX = vars.vertWallHitX;
    if (vars.horzHitDistance < vars.vertHitDistance)
        vars.wallHitX = vars.horzWallHitX;
    vars.wallHitY = vars.vertWallHitY;
    if (vars.horzHitDistance < vars.vertHitDistance)
        vars.wallHitY = vars.horzWallHitY;
    if (is == 2)
        DDA(cube, circle_center_x, circle_center_y, vars.wallHitX, vars.wallHitY);
    return (vars);
}

void    draw_lines(t_cub *cube, double circle_center_x, double circle_center_y)
{
    double angle;

    angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
    int i = 0;
    double tmp = (double)cube->data->width / NUM_RAYS;
    (void)tmp;
    // printf("[%d]\n", tmp);
    while (i < NUM_RAYS)
    {

        printf("[%f]\n", angle);
        draw_line(cube, circle_center_x, circle_center_y, angle, 2);
        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
}

unsigned int rgba_to_int(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

void draw_lines_3D(t_cub *cube)
{
    int i = 0;
    double angle;

    angle = cube->player->rotat_angle + FOV_ANGLE / 2;
    int x = cube->data->width / NUM_RAYS;
    while (i < NUM_RAYS)
    {
        t_vars vars = draw_line(cube, cube->player->player_x, cube->player->player_y, angle, 1);

        //////
        double distanceprojplane = ((cube->data->width) / 2) / (tan(FOV_ANGLE / 2));
        double wallDistance = vars.distance * cos(angle - cube->player->rotat_angle);
        double projectedwallhight = (tile_size / wallDistance) * distanceprojplane;

        int wallstripheight = (int)projectedwallhight;
        // start wall
        int wallTopPixel = ((cube->data->height) / 2) - (wallstripheight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
        int wallBottompixel = ((cube->data->height) / 2) + (wallstripheight / 2);
        wallBottompixel = wallBottompixel > (cube->data->height)  ? (cube->data->height) : wallBottompixel;
        // end wall
        // double shading = 2 / (2 + wallDistance * 0.03);
        int color = vars.wasHitVert ? 200 : 255;
        int base = 1.0;
        x += i;
        for (int y = wallTopPixel; y < wallBottompixel ; y++)
        {
            my_mlx_pixel_put(&cube->img, x, y, rgba_to_int(color, color, color, base));
        }

        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
}


void draw_view_player(t_cub *cube, int is)
{
    // Update the player's rotation angle
    cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
    cube->player->rotat_angle += (double)cube->player->turn_direction * cube->player->rotation_speed;

    // Calculate the movement step in both x and y directions
    double movestep = (double)cube->player->walk_direction * cube->player->move_speed;

    // Calculate new player position based on current angle
    double new_player_x = cube->player->player_x + ((double)movestep * cos(cube->player->rotat_angle));
    double new_player_y = cube->player->player_y + ((double)movestep * sin(cube->player->rotat_angle));

    // Check if the new position is within the bounds and not a wall
    if (is_it_a_wall(new_player_x, new_player_y, cube) == 1)
    {
        cube->player->player_x = new_player_x;
        cube->player->player_y = new_player_y;
    }
    if (is == 2)
    {
        // draw_player(cube, cube->player->player_x, cube->player->player_y);
        draw_player(cube);

        // Draw a single line to represent the player's view
        draw_lines(cube, cube->player->player_x, cube->player->player_y);
        draw_line(cube, cube->player->player_x, cube->player->player_y, cube->player->rotat_angle, 2);
    }
    else
        draw_lines_3D(cube);
    // Draw the player at the new position

    // Uncomment if you want to draw multiple lines (for a field of view representation)
}


void	handle_pixel2(int x, int y, t_cub *cube,int is)
{
    if(cube->data->map[y][x] == 'N')
    {
        draw_cube(cube, x, y, BLACK, 1);
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
//end draw_player

// draw_map
void	handle_pixel(int x, int y, t_cub *cube)
{
    if(cube->data->map[y][x] == '1')
        draw_cube(cube, x, y, WHITE, 1);
    else if(cube->data->map[y][x] == '0')
        draw_cube(cube, x, y, BLACK, 1);
}

void	draw_map(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
			handle_pixel(x, y, cube);
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
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
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


void move_player(t_cub *cube, int direction) {
    // direction: 1 for moving right, -1 for moving left
    double moveStep = direction * cube->player->move_speed;
    double new_player_x = cube->player->player_x + (moveStep * cos(cube->player->rotat_angle + M_PI_2));
    double new_player_y = cube->player->player_y + (moveStep * sin(cube->player->rotat_angle + M_PI_2));

    // Check if the new position is a wall or not
    if (is_it_a_wall(new_player_x, new_player_y, cube)) {
        cube->player->player_x = new_player_x;
        cube->player->player_y = new_player_y;
    }
}




// Modify your key handling function to use the move_player function

int handle_input_key_down(int keycode, t_cub *cube) {
    if (keycode == 13)
        cube->player->walk_direction = 1;
    if (keycode == 1)
        cube->player->walk_direction = -1;
    if (keycode == 2)
        move_player(cube, 1);
    if (keycode == 0)
        move_player(cube, -1);
    if (keycode == 124)
        cube->player->turn_direction = 1;
    if (keycode == 123)
        cube->player->turn_direction = -1;
    if (keycode == 53)
	{
		mlx_destroy_window(cube->mlx_con, cube->mlx_win);
		free(cube->mlx_con);
		exit(1);
	}
    return 0;
}

int handle_input_key_up(int keycode, t_cub *cube) {
    if (keycode == 13 || keycode == 1)
        cube->player->walk_direction = 0;
    else if (keycode == 124 || keycode == 123)
        cube->player->turn_direction = 0;
    return 0;
}


int loop_fun(t_cub * cube)
{
    draw_all_black(cube);
    // update_player(cube);
    draw_per(cube, 2);
    draw_map(cube);
    draw_per(cube, 1);
    // mlx_clear_window(cube->mlx_con, cube->mlx_win);
	mlx_put_image_to_window(cube->mlx_con, cube->mlx_win, cube->img.img_ptr, 0, 0);
    return (0);
}
// end hooks
