#include "recasting.h"
#include <time.h>

void	malloc_error(void)
{
	printf("error in malloc\n");
	exit(EXIT_FAILURE);
}

double deg2rad(double degrees)
{
    return degrees * (M_PI / 180.0);
}

t_player *init_player(t_cub *cube)
{
    int x;
    int y;
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
            if (cube->data->map[y][x] == 'N')
            {
                player->player_x = x * (tile_size * MAP_SCALE) + ((tile_size * MAP_SCALE) / 2);
                player->player_y = y * (tile_size * MAP_SCALE) + ((tile_size * MAP_SCALE) / 2);
            }
        }
    }
    player->radius = 10;
    player->move_speed = 7;
    player->rotat_angle = deg2rad(90);  // Initialize in radians
    player->rotation_speed = 0.05;
    player->turn_direction = 0;
    player->strafe_direction = 0;
    player->walk_direction = 0;

    return (player);
}

void ft_fractol_init(t_cub *cube)
{
    cube->mlx_con = mlx_init();
    if (!cube->mlx_con)
        malloc_error();
    cube->mlx_win = mlx_new_window(cube->mlx_con, cube->data->width, cube->data->height, "Cub3D");
    if (!cube->mlx_win)
        malloc_error();
    cube->img.img_ptr = mlx_new_image(cube->mlx_con, cube->data->width, cube->data->height);
    if (!cube->img.img_ptr)
    {
        mlx_destroy_window(cube->mlx_con, cube->mlx_win);
        malloc_error();
    }
    cube->img.pixels_ptr = mlx_get_data_addr(cube->img.img_ptr, &cube->img.bpp, &cube->img.line_len, &cube->img.endian);
    if (!cube->img.pixels_ptr)
    {
        mlx_destroy_image(cube->mlx_con, cube->img.img_ptr);
        mlx_destroy_window(cube->mlx_con, cube->mlx_win);
        malloc_error();
    }

    // Load multiple textures
    char *texture_files[] = {"wood0.xpm", "wood1.xpm", "wood2.xpm", "wood3.xpm"};
    for (int i = 0; i < 4; i++) {
        cube->texture[i].img_ptr = mlx_xpm_file_to_image(cube->mlx_con, texture_files[i], &cube->texture[i].width, &cube->texture[i].height);
        if (!cube->texture[i].img_ptr)
            malloc_error();
        cube->texture[i].data = mlx_get_data_addr(cube->texture[i].img_ptr, &cube->texture[i].bpp, &cube->texture[i].line_len, &cube->texture[i].endian);
        if (!cube->texture[i].data)
        {
            mlx_destroy_image(cube->mlx_con, cube->texture[i].img_ptr);
            malloc_error();
        }
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

// all_black
void	handle_pixel3(int x, int y, t_cub *cube)
{

	int	i;
	int	j;
	j = -1;
    while (++j < (tile_size))
    {
        i = -1;
        while (++i < (tile_size))
            my_mlx_pixel_put(&cube->img, (x * (tile_size)) + i , (y * (tile_size)) + j, BLACK);
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

// draw_player
int is_it_a_wall(double x, double y, t_cub *cube)
{
    double left = x - ((double)cube->player->radius  * MAP_SCALE);
    double up = y + ((double)cube->player->radius  * MAP_SCALE);
    double right = x + ((double)cube->player->radius  * MAP_SCALE);
    double down = y - ((double)cube->player->radius  * MAP_SCALE);

    if(left < 0 || right > cube->data->map_cols * (tile_size * MAP_SCALE) || up < 0 || down > cube->data->map_row * (tile_size * MAP_SCALE))
        return (0);

    int  t_left = floor(left / (tile_size * MAP_SCALE));
    int  t_up = floor(up / (tile_size * MAP_SCALE));
    int  t_right = floor(right / (tile_size * MAP_SCALE));
    int  t_down = floor(down / (tile_size * MAP_SCALE));

    if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
        || cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
        return (0);
    return (1);
}


void draw_player(t_cub *cube)
{
    int radius = cube->player->radius * MAP_SCALE;
    double centerX = cube->player->player_x;
    double centerY = cube->player->player_y;

    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
            if (x * x + y * y <= radius * radius)
                my_mlx_pixel_put(&cube->img, centerX + x, centerY + y, BLUE);
    }
}




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

int has_wall(t_cub *cube, double x1, double y1)
{
    int x = floor(x1 / (tile_size * MAP_SCALE));
    int y = floor(y1 / (tile_size * MAP_SCALE));

    if (cube->data->map[y][x] == '1')
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



void ft_draw_hero(t_cub *cube, t_vars *vars) {
    vars->y_intercept = floor(cube->player->player_y / (tile_size * MAP_SCALE)) * (tile_size * MAP_SCALE);
    if (vars->isRayFacingDown)
        vars->y_intercept += (tile_size * MAP_SCALE);

    vars->x_intercept = cube->player->player_x + (vars->y_intercept - cube->player->player_y) / tan(vars->angle);

    vars->ystep = (tile_size * MAP_SCALE);
    if (vars->isRayFacingUp)
        vars->ystep *= -1;

    vars->xstep = (tile_size * MAP_SCALE) / tan(vars->angle);
    if (vars->isRayFacingLeft && vars->xstep > 0)
        vars->xstep *= -1;
    if (vars->isRayFacingRight && vars->xstep < 0)
        vars->xstep *= -1;

    vars->nextHorzTouchX = vars->x_intercept;
    vars->nextHorzTouchY = vars->y_intercept;

    if (vars->isRayFacingUp)
        vars->nextHorzTouchY--;

    while (vars->nextHorzTouchX >= 0 && vars->nextHorzTouchX < cube->data->map_cols * (tile_size * MAP_SCALE) &&
           vars->nextHorzTouchY >= 0 && vars->nextHorzTouchY < cube->data->map_row * (tile_size * MAP_SCALE)) {
        if (has_wall(cube, vars->nextHorzTouchX, vars->nextHorzTouchY)) {
            vars->foundHorzWallHit = 1;
            vars->horzWallHitX = vars->nextHorzTouchX;
            vars->horzWallHitY = vars->nextHorzTouchY;
            break;
        } else {
            vars->nextHorzTouchX += vars->xstep;
            vars->nextHorzTouchY += vars->ystep;
        }
    }
}

void ft_draw_ver(t_cub *cube, t_vars *vars) {
    vars->foundVertWallHit = 0;
    vars->vertWallHitX = 0;
    vars->vertWallHitY = 0;

    vars->x_intercept = floor(cube->player->player_x / (tile_size * MAP_SCALE)) * (tile_size * MAP_SCALE);
    if (vars->isRayFacingRight)
        vars->x_intercept += (tile_size * MAP_SCALE);

    vars->y_intercept = cube->player->player_y + (vars->x_intercept - cube->player->player_x) * tan(vars->angle);

    vars->xstep = (tile_size * MAP_SCALE);
    if (vars->isRayFacingLeft)
        vars->xstep *= -1;

    vars->ystep = (tile_size * MAP_SCALE) * tan(vars->angle);
    if (vars->isRayFacingUp && vars->ystep > 0)
        vars->ystep *= -1;
    if (vars->isRayFacingDown && vars->ystep < 0)
        vars->ystep *= -1;

    vars->nextVertTouchX = vars->x_intercept;
    vars->nextVertTouchY = vars->y_intercept;

    if (vars->isRayFacingLeft)
        vars->nextVertTouchX--;

    while (vars->nextVertTouchX >= 0 && vars->nextVertTouchX < cube->data->map_cols * (tile_size * MAP_SCALE) &&
           vars->nextVertTouchY >= 0 && vars->nextVertTouchY < cube->data->map_row * (tile_size * MAP_SCALE)) {
        if (has_wall(cube, vars->nextVertTouchX, vars->nextVertTouchY)) {
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


double distanceBetweenPoints(double x1,double y1,double x2,double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

t_vars draw_line(t_cub *cube, double angle, int is) {
    t_vars vars;
    vars.angle = normalizeAngle(angle);

    vars.isRayFacingDown = vars.angle > 0 && vars.angle < M_PI;
    vars.isRayFacingUp = !vars.isRayFacingDown;
    vars.isRayFacingRight = vars.angle < 0.5 * M_PI || vars.angle > 1.5 * M_PI;
    vars.isRayFacingLeft = !vars.isRayFacingRight;

    vars.foundHorzWallHit = 0;
    vars.horzWallHitX = 0;
    vars.horzWallHitY = 0;

    ft_draw_hero(cube, &vars);
    ft_draw_ver(cube, &vars);
    vars.horzHitDistance = __DBL_MAX__;
    vars.vertHitDistance = __DBL_MAX__;

    if (vars.foundHorzWallHit)
        vars.horzHitDistance = distanceBetweenPoints(cube->player->player_x, cube->player->player_y, vars.horzWallHitX, vars.horzWallHitY);
    if (vars.foundVertWallHit)
        vars.vertHitDistance = distanceBetweenPoints(cube->player->player_x, cube->player->player_y, vars.vertWallHitX, vars.vertWallHitY);

    vars.wallHitX = vars.vertWallHitX;
    vars.wallHitY = vars.vertWallHitY;
    vars.distance = vars.vertHitDistance;
    vars.wasHitVert = 1;

    if (vars.vertHitDistance < vars.horzHitDistance) {
        vars.wallHitX = vars.vertWallHitX;
        vars.wallHitY = vars.vertWallHitY;
        vars.distance = vars.vertHitDistance;
        vars.wasHitVert = 1;
    } else {
        vars.wallHitX = vars.horzWallHitX;
        vars.wallHitY = vars.horzWallHitY;
        vars.distance = vars.horzHitDistance;
        vars.wasHitVert = 0;
    }

    if (is == 2)
        DDA(cube, cube->player->player_x, cube->player->player_y, vars.wallHitX, vars.wallHitY);

    return vars;
}



void draw_lines(t_cub *cube) {
    double angle = cube->player->rotat_angle + FOV_ANGLE / 2;
    int i = 0;
    while (i < NUM_RAYS) {
        draw_line(cube, angle, 2);
        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
}

void draw_lines_3D(t_cub *cube) {
    int i = 0;
    double angle;
    double distanceprojplane = ((double)cube->data->width / 2.0) / tan(FOV_ANGLE / 2);

    angle = cube->player->rotat_angle - FOV_ANGLE / 2.0;
    double rayWidth = (double)cube->data->width / NUM_RAYS;

    while (i < NUM_RAYS) {
        t_vars vars = draw_line(cube, angle, 1);

        double wallDistance = vars.distance * cos(angle - cube->player->rotat_angle);
        double projectedwallheight = (tile_size / wallDistance) * distanceprojplane;

        int wallstripheight = (int)projectedwallheight;
        int wallTopPixel = (cube->data->height / 2) - (wallstripheight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
        int wallBottompixel = (cube->data->height / 2) + (wallstripheight / 2);
        wallBottompixel = wallBottompixel > cube->data->height ? cube->data->height : wallBottompixel;

        double textureStep = (double)cube->texture[0].height / wallstripheight;
        double textureOffsetY = 0;

        if (wallstripheight > cube->data->height) {
            textureOffsetY = ((double)wallstripheight - (double)cube->data->height) / 2.0;
            wallstripheight = (double)cube->data->height;
        }

        int textureNum = 0;
        if (vars.wasHitVert) {
            if (vars.isRayFacingLeft)
                textureNum = 2; // West
            else
                textureNum = 3; // East
        } else {
            if (vars.isRayFacingUp)
                textureNum = 0; // North
            else
                textureNum = 1; // South
        }

        t_texture *texture = &cube->texture[textureNum];
        int textureX;
        if (vars.wasHitVert)
            textureX = (int)vars.wallHitY % tile_size;
        else
            textureX = (int)vars.wallHitX % tile_size;

        double texturePos = textureOffsetY * textureStep;

        int x = i * rayWidth;
        for (int y = wallTopPixel; y < wallBottompixel; y++) {
            int textureY = (int)texturePos & (texture->height - 1);
            texturePos += textureStep;

            int color = *(unsigned int *)(texture->data + (textureY * texture->line_len + textureX * (texture->bpp / 8)));
            // Apply shading
            double shadeFactor = 20 / (20 + wallDistance * 0.1);
            int r = ((color >> 16) & 0xFF) * shadeFactor;
            int g = ((color >> 8) & 0xFF) * shadeFactor;
            int b = (color & 0xFF) * shadeFactor;
            int shadedColor = (r << 16) | (g << 8) | b;
            my_mlx_pixel_put(&cube->img, x, y, shadedColor);
            // my_mlx_pixel_put(&cube->img, x, y, get_texture_color(texture, textureX, textureY));
        }

        angle += FOV_ANGLE / NUM_RAYS;
        i++;
    }
}



int create_rgb_color(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

unsigned int rgba_to_int(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}


void draw_view_player(t_cub *cube, int is)
{
    // Normalize the player's rotation angle
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
    if (is_it_a_wall(new_player_x, new_player_y, cube) == 1)
    {
        cube->player->player_x = new_player_x;
        cube->player->player_y = new_player_y;
    }

    if (is == 1)
    {
        // Draw the player and the 2D map view
        draw_player(cube);
        draw_lines(cube);
    }
    else
        // Draw the 3D view
        draw_lines_3D(cube);
}

void	handle_pixel2(int x, int y, t_cub *cube, int is)
{
    if(cube->data->map[y][x] == 'N')
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
//end draw_player

// draw_map

void handle_pixel(int x, int y, t_cub *cube) {
    // Compute the screen coordinates based on player's position
    if (cube->data->map[y][x] == '1')
        draw_cube(cube, x, y, WHITE, 1);
    else if (cube->data->map[y][x] == '0')
        draw_cube(cube, x, y, RED, 1);
}

void draw_map(t_cub *cube) {
    int x, y;
    y = -1;
    while (++y < cube->data->map_row)
    {
        x = -1;
        while (++x < cube->data->map_cols)
            handle_pixel(x, y, cube);
    }
}

// end draw_map

// hooks
int	handle_close_button(t_cub *data)
{
	mlx_destroy_window(data->mlx_con, data->mlx_win);
	exit(1);
}


// Modify your key handling function to use the move_player function


int handle_input_key_down(int keycode, t_cub *cube)
{
    if (keycode == 13)
        cube->player->walk_direction = 1;
    if (keycode == 1)
        cube->player->walk_direction = -1;
    if (keycode == 0)
        cube->player->strafe_direction = -1;
    if (keycode == 2)
        cube->player->strafe_direction = 1;
    if (keycode == 123)
        cube->player->turn_direction = -1;
    if (keycode == 124)
        cube->player->turn_direction = 1;
    if (keycode == 53)
	{
		mlx_destroy_window(cube->mlx_con, cube->mlx_win);
		free(cube->mlx_con);
		exit(1);
	}
    return (0);
}

int handle_input_key_up(int keycode, t_cub *cube)
{
    if (keycode == 13 || keycode == 1)
        cube->player->walk_direction = 0;
    if (keycode == 2 || keycode == 0)
        cube->player->strafe_direction = 0;
    if (keycode == 124 || keycode == 123)
        cube->player->turn_direction = 0;
    return (0);
}

// int	ft_mouse_move(int x, int y, t_cub *cube)
// {
//     (void)cube;
//     (void)y;
//     if (x < cube->data->width && x > 0 && y < cube->data->height && y > 0)
//     {
//          static int last_x = -1;
//         if (last_x == -1)
//             last_x = x;

//         int delta_x = x - last_x;
//         last_x = x;

//         cube->player->rotat_angle += delta_x * 0.03;  // Adjust sensitivity as needed
//         cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);

//         // Redraw the scene with the updated player rotation
//         draw_per(cube, 0);
//     }
//     return (1);
// }



int loop_fun(t_cub * cube)
{
    // mlx_clear_window(cube->mlx_con, cube->mlx_win);
    draw_all_black(cube);
    // ft_render3d_walls();
    draw_per(cube, 2);
    // draw_map(cube);
    // draw_per(cube, 1);
    // draw_all_black(cube);
	mlx_put_image_to_window(cube->mlx_con, cube->mlx_win, cube->img.img_ptr, 0, 0);
    return (0);
}
// end hooks
