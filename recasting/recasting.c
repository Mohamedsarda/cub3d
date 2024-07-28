#include "recasting.h"
#include <time.h>
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	malloc_error(void)
{
	printf("Error: in malloc\n");
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
            if (cube->data->map[y][x] == cube->data->p)
            {
                player->player_x = x * tile_size + (tile_size / 2);
                player->player_y = y * tile_size + (tile_size / 2);
            }
        }
    }
    player->radius = 10;
    player->move_speed = 7;
    if (cube->data->p == 'W')
        player->rotat_angle = deg2rad(180);  // Initialize in radians
    else if (cube->data->p == 'S')
        player->rotat_angle = deg2rad(90);  // Initialize in radians
    else if (cube->data->p == 'N')
        player->rotat_angle = deg2rad(270);  // Initialize in radians
    else if (cube->data->p == 'E')
        player->rotat_angle = deg2rad(0);  // Initialize in radians
    player->rotation_speed = 0.05;
    player->turn_direction = 0;
    player->strafe_direction = 0;
    player->walk_direction = 0;

    return (player);
}

int create_rgba(int r, int g, int b, int a) {
    return (r << 24) | (g << 16) | (b << 8) | a;
}

void ft_fractol_init(t_cub *cube)
{
   cube->mlx = mlx_init(cube->data->width, cube->data->height, "42Balls", true);
	if (!cube->mlx)
		ft_error();

	cube->image = mlx_new_image(cube->mlx, cube->data->width, cube->data->height);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();

    // cube->mlx_con = mlx_init();
    // if (!cube->mlx_con)
    //     malloc_error();
    // cube->mlx_win = mlx_new_window(cube->mlx_con, cube->data->width, cube->data->height, "Cub3D");
    // if (!cube->mlx_win)
    //     malloc_error();
    // cube->img.img_ptr = mlx_new_image(cube->mlx_con, cube->data->width, cube->data->height);
    // if (!cube->img.img_ptr)
    // {
    //     mlx_destroy_window(cube->mlx_con, cube->mlx_win);
    //     malloc_error();
    // }
    // cube->img.pixels_ptr = mlx_get_data_addr(cube->img.img_ptr, &cube->img.bpp, &cube->img.line_len, &cube->img.endian);
    // if (!cube->img.pixels_ptr)
    // {
    //     mlx_destroy_image(cube->mlx_con, cube->img.img_ptr);
    //     mlx_destroy_window(cube->mlx_con, cube->mlx_win);
    //     malloc_error();
    // }

    // Load multiple textures
    // char *texture_files[] = {"wall.xpm", "wall.xpm", "wall.xpm", "wall.xpm"};
    // char *texture_files[] = {"./wood0.xpm", "./wood1.xpm", "./wood2.xpm", "./wood3.xpm"};
    // char *texture_files[] = {cube->data->no, cube->data->so, cube->data->we, cube->data->ea};
    // // mlx_load_xpm42
    // for (int i = 0; i < 4; i++)
    // {
    //     cube->texture[i] = mlx_load_png(texture_files[i]);
    //     cube->img[i] = mlx_texture_to_image(cube->mlx, cube->texture[i]);
    //     if (mlx_image_to_window(cube->mlx, cube->img[i], 0, 0) < 0)
    //         ft_error();
    // }

    cube->player = init_player(cube);
}

// void my_mlx_pixel_put(t_img *img, int x, int y, int color)
// {
// 	int	offset;
// 	offset = (y * img->line_len) + (x * (img->bpp / 8));
// 	*(unsigned int *)(img->pixels_ptr + offset) = color;
// }

void    draw_cube(t_cub *cube, int x, int y, int color)
{
	int	i;
	int	j;

	j = -1;
    while (++j < ((tile_size * MAP_SCALE) -1))
    {
        i = -1;
        while (++i < ((tile_size * MAP_SCALE) -1))
            mlx_put_pixel(cube->image,  (x * ((tile_size) * MAP_SCALE)) + i ,  (y * ((tile_size) * MAP_SCALE)) + j, color);
    }
    return ;
}

// all_black
void	handle_pixel3(int x, int y, t_cub *cube)
{

	int	i;
	int	j;
	j = -1;
    while (++j < (tile_size - 1))
    {
        i = -1;
        while (++i < (tile_size - 1))
            mlx_put_pixel(cube->image, (x * tile_size) + i , (y * tile_size) + j, create_rgba(0, 0, 0, 150));
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
int is_it_a_wall(double x, double y, t_cub *cube, int is)
{
    double scale = MAP_SCALE;
    if (is != 1)
        scale = 1;
    double left = x - ((double)cube->player->radius * scale);
    double up = y + ((double)cube->player->radius * scale);
    double right = x + ((double)cube->player->radius * scale);
    double down = y - ((double)cube->player->radius * scale);

    // printf("left = %f | right = %f | up = %f | down = %f | width = %d | he = %d | radius = %d | x = %f | y = %f\n", left, right, up, down, cube->data->width, cube->data->height, cube->player->radius, x, y);
    if(left < 0 || right > cube->data->map_cols * (tile_size) || up < 0 || down > cube->data->map_row * (tile_size))
        return (0);

    int  t_left = floor(left / (tile_size));
    int  t_up = floor(up / (tile_size));
    int  t_right = floor(right / (tile_size));
    int  t_down = floor(down / (tile_size));

    if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
        || cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
        return (0);
    return (1);
}


    // printf("%f %f\n", centerX , centerY);
void draw_player(t_cub *cube)
{
    double radius = (double)cube->player->radius * MAP_SCALE;
    double centerX = cube->player->player_x * MAP_SCALE;
    double centerY = cube->player->player_y * MAP_SCALE;
    // printf("%f | %f | %f | %f | %f\n",cube->player->player_x, cube->player->player_y, cube->player->player_x * MAP_SCALE, cube->player->player_y * MAP_SCALE, radius);
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
            if (x * x + y * y <= radius * radius)
                mlx_put_pixel(cube->image, centerX + (double)x , centerY + (double)y, RED);
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
    for (int i = 0; i <= steps; i++)
    {
        mlx_put_pixel(cube->image, X , Y, create_rgba(250, 100, 100 , 255));
        X += Xinc; // increment in x at each step
        Y += Yinc; // increment in y at each step
    }
}

int has_wall(t_cub *cube, double x1, double y1)
{
    int x = floor(x1 / (tile_size));
    int y = floor(y1 / (tile_size));

    if (cube->data->map[y][x] == '1')
        return (1);
    return (0);
}

double normalizeAngle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
    {
        angle += 2 * M_PI;
    }
    return angle;
}

void ft_draw_hero(t_cub *cube, t_vars *vars)
{
    vars->y_intercept = floor(cube->player->player_y / (tile_size)) * (tile_size);
    if (vars->isRayFacingDown)
        vars->y_intercept += (tile_size);

    vars->x_intercept = cube->player->player_x + (vars->y_intercept - cube->player->player_y) / tan(vars->angle);

    vars->ystep = (tile_size);
    if (vars->isRayFacingUp)
        vars->ystep *= -1;

    vars->xstep = (tile_size) / tan(vars->angle);
    if (vars->isRayFacingLeft && vars->xstep > 0)
        vars->xstep *= -1;
    if (vars->isRayFacingRight && vars->xstep < 0)
        vars->xstep *= -1;

    vars->nextHorzTouchX = vars->x_intercept;
    vars->nextHorzTouchY = vars->y_intercept;

    if (vars->isRayFacingUp)
        vars->nextHorzTouchY--;

    while (vars->nextHorzTouchX >= 0 && vars->nextHorzTouchX < cube->data->map_cols * (tile_size) &&
           vars->nextHorzTouchY >= 0 && vars->nextHorzTouchY < cube->data->map_row * (tile_size))
           {
        if (has_wall(cube, vars->nextHorzTouchX, vars->nextHorzTouchY))
        {
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

void ft_draw_ver(t_cub *cube, t_vars *vars)
{
    vars->foundVertWallHit = 0;
    vars->vertWallHitX = 0;
    vars->vertWallHitY = 0;

    vars->x_intercept = floor(cube->player->player_x / (tile_size)) * (tile_size);
    if (vars->isRayFacingRight)
        vars->x_intercept += (tile_size);

    vars->y_intercept = cube->player->player_y + (vars->x_intercept - cube->player->player_x) * tan(vars->angle);

    vars->xstep = (tile_size);
    if (vars->isRayFacingLeft)
        vars->xstep *= -1;

    vars->ystep = (tile_size) * tan(vars->angle);
    if (vars->isRayFacingUp && vars->ystep > 0)
        vars->ystep *= -1;
    if (vars->isRayFacingDown && vars->ystep < 0)
        vars->ystep *= -1;

    vars->nextVertTouchX = vars->x_intercept;
    vars->nextVertTouchY = vars->y_intercept;

    if (vars->isRayFacingLeft)
        vars->nextVertTouchX--;

    while (vars->nextVertTouchX >= 0 && vars->nextVertTouchX < cube->data->map_cols * (tile_size) &&
           vars->nextVertTouchY >= 0 && vars->nextVertTouchY < cube->data->map_row * (tile_size))
           {
        if (has_wall(cube, vars->nextVertTouchX, vars->nextVertTouchY))
        {
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

double distanceBetweenPoints(double x1,double y1,double x2,double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

t_vars draw_line(t_cub *cube, double angle, int is)
{
    t_vars vars;
    vars.angle = normalizeAngle(angle);
    (void)is;
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

    if (vars.vertHitDistance < vars.horzHitDistance)
    {
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
    if (cube->is == 0.25)
        DDA(cube, cube->player->player_x * cube->is, cube->player->player_y * cube->is, vars.wallHitX * cube->is, vars.wallHitY * cube->is);

    return vars;
}

void draw_lines(t_cub *cube, int is)
{
    cube->is = 0.25;
    double angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
    int i = 0;
    while (i < NUM_RAYS)
    {
        draw_line(cube, angle, is);
        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
}

// void draw_lines_3D(t_cub *cube)
// {
//     int i = 0;
//     double angle;
//     double distanceprojplane = ((double)cube->data->width / 2.0) / tan(FOV_ANGLE / 2);

//     angle = cube->player->rotat_angle - FOV_ANGLE / 2.0;
//     double rayWidth = (double)cube->data->width / NUM_RAYS;

//     while (i < NUM_RAYS)
//     {
//         t_vars vars = draw_line(cube, angle, 1);

//         double wallDistance = vars.distance * cos(angle - cube->player->rotat_angle);
//         double projectedwallheight = (tile_size / wallDistance) * distanceprojplane;

//         int wallstripheight = (int)projectedwallheight;
//         int wallTopPixel = (cube->data->height / 2) - (wallstripheight / 2);
//         wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
//         int wallBottompixel = (cube->data->height / 2) + (wallstripheight / 2);
//         wallBottompixel = wallBottompixel > cube->data->height ? cube->data->height : wallBottompixel;

//         double textureStep = (double)cube->texture[0].height / wallstripheight;
//         double textureOffsetY = 0;

//         if (wallstripheight > cube->data->height)
//         {
//             textureOffsetY = ((double)wallstripheight - (double)cube->data->height) / 2.0;
//             wallstripheight = (double)cube->data->height;
//         }

//         int textureNum = 0;
//         if (vars.wasHitVert)
//         {
//             if (vars.isRayFacingLeft)
//                 textureNum = 2; // West
//             else
//                 textureNum = 3; // East
//         } else {
//             if (vars.isRayFacingUp)
//                 textureNum = 0; // North
//             else
//                 textureNum = 1; // South
//         }

//         t_texture *texture = &cube->texture[textureNum];
//         int textureX;
//         if (vars.wasHitVert)
//             textureX = (int)vars.wallHitY % tile_size;
//         else
//             textureX = (int)vars.wallHitX % tile_size;

//         double texturePos = textureOffsetY * textureStep;

//         int x = i * rayWidth;
//         for (int y = wallTopPixel; y < wallBottompixel; y++)
//         {
//             int textureY = (int)texturePos & (texture->height - 1);
//             texturePos += textureStep;

//             int color = *(unsigned int *)(texture->data + (textureY * texture->line_len + textureX * (texture->bpp / 8)));
//             // Apply shading
//             double shadeFactor = 20 / (20 + wallDistance * 0.1);
//             int r = ((color >> 16) & 0xFF) * shadeFactor;
//             int g = ((color >> 8) & 0xFF) * shadeFactor;
//             int b = (color & 0xFF) * shadeFactor;
//             int shadedColor = (r << 16) | (g << 8) | b;
//             mlx_put_pixel(cube->image, x, y, shadedColor);
//             // my_mlx_pixel_put(&cube->img, x, y, get_texture_color(texture, textureX, textureY));
//         }

//         angle += FOV_ANGLE / NUM_RAYS;
//         i++;
//     }
// }
// void draw_lines_3D(t_cub *cube)
// {
//     int i = 0;
//     double angle;
//     double distanceprojplane = ((double)cube->data->width / 2.0) / tan(FOV_ANGLE / 2);

//     angle = cube->player->rotat_angle - FOV_ANGLE / 2.0;
//     double rayWidth = (double)cube->data->width / NUM_RAYS;

//     while (i < NUM_RAYS)
//     {
//         t_vars vars = draw_line(cube, angle, 1);

//         double wallDistance = vars.distance * cos(angle - cube->player->rotat_angle);
//         double projectedwallheight = (tile_size / wallDistance) * distanceprojplane;

//         int wallstripheight = (int)projectedwallheight;
//         int wallTopPixel = (cube->data->height / 2) - (wallstripheight / 2);
//         wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
//         int wallBottompixel = (cube->data->height / 2) + (wallstripheight / 2);
//         wallBottompixel = wallBottompixel > cube->data->height ? cube->data->height : wallBottompixel;

//         // double textureStep = (double)cube->image[0].height / wallstripheight;
//         double textureOffsetY = 0;

//         if (wallstripheight > cube->data->height)
//         {
//             textureOffsetY = ((double)wallstripheight - (double)cube->data->height) / 2.0;
//             wallstripheight = (double)cube->data->height;
//         }

//         // int textureNum = 0;
//         // if (vars.wasHitVert)
//         // {
//         //     if (vars.isRayFacingLeft)
//         //         textureNum = 2; // West
//         //     else
//         //         textureNum = 3; // East
//         // } else {
//         //     if (vars.isRayFacingUp)
//         //         textureNum = 0; // North
//         //     else
//         //         textureNum = 1; // South
//         // }
//         int x = i * rayWidth;

//         //get textures

//         angle += FOV_ANGLE / NUM_RAYS;
//         i++;
//     }
// }

int create_rgb_color(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

// unsigned int create_rgba (unsigned char r, unsigned char g, unsigned char b, unsigned char a)
// {
//     return (a << 24) | (r << 16) | (g << 8) | b;
// }

void draw_view_player(t_cub *cube, int is)
{
    // Normalize the player's rotation angle
    cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
    cube->player->rotat_angle += (double)cube->player->turn_direction  * cube->player->rotation_speed;

    // Calculate movement step
    double movestep = (double)cube->player->walk_direction * cube->player->move_speed;
    double new_player_x;
    double new_player_y;
    // (void)is;
    // new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
    // new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);
    if (is == 1)
    {
        new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
        new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);
    }
    else
    {
        new_player_x = (cube->player->player_x / MAP_SCALE) + movestep * cos(cube->player->rotat_angle);
        new_player_y = (cube->player->player_y / MAP_SCALE) + movestep * sin(cube->player->rotat_angle);
    }

    // Handle strafe movement (left/right)
    if (cube->player->strafe_direction != 0)
    {
        new_player_x += (double)cube->player->strafe_direction * (cube->player->move_speed / 2) * cos(cube->player->rotat_angle + M_PI / 2);
        new_player_y += (double)cube->player->strafe_direction * (cube->player->move_speed / 2) * sin(cube->player->rotat_angle + M_PI / 2);
    }

    // Check if the new position is within bounds and not a wall
    if (is_it_a_wall(new_player_x, new_player_y, cube, is))
    {
        cube->player->player_x = new_player_x;
        cube->player->player_y = new_player_y;
    }
    else
    {
        // Try sliding along the wall
        if (is_it_a_wall(cube->player->player_x, new_player_y, cube, is))
            cube->player->player_y = new_player_y;
        else if (is_it_a_wall(new_player_x, cube->player->player_y, cube, is))
            cube->player->player_x = new_player_x;
    }

    // draw_lines_3D(cube);
}

void	handle_pixel2(int x, int y, t_cub *cube)
{
    if(cube->data->map[y][x] == cube->data->p)
    {
        draw_cube(cube, x, y, create_rgba(0, 0, 255, 255));
        draw_view_player(cube, 1);
        draw_player(cube);
        draw_lines(cube, 1);
    }
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
// //end draw_player

// // draw_map

void handle_pixel(int x, int y, t_cub *cube)
{
    // Compute the screen coordinates based on player's position
    if (cube->data->map[y][x] == '1')
        draw_cube(cube, x, y, create_rgba(255, 255, 255, 10));
    else if (cube->data->map[y][x] == '0')
        draw_cube(cube, x, y, create_rgba(255, 255, 255, 255));
    //  || cube->data->map[y][x] == cube->data->p
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

// // end draw_map

// // hooks
// int	handle_close_button(t_cub *data)
// {
// 	mlx_destroy_window(data->mlx_con, data->mlx_win);
// 	exit(1);
// }


// // Modify your key handling function to use the move_player function

// int handle_input_key_down(int keycode, t_cub *cube)
// {
//     if (keycode == 13)
//         cube->player->walk_direction = 1;
//     if (keycode == 1)
//         cube->player->walk_direction = -1;
//     if (keycode == 0)
//         cube->player->strafe_direction = -1;
//     if (keycode == 2)
//         cube->player->strafe_direction = 1;
//     if (keycode == 123)
//         cube->player->turn_direction = -1;
//     if (keycode == 124)
//         cube->player->turn_direction = 1;
//     if (keycode == 53)
// 	{
// 		mlx_destroy_window(cube->mlx_con, cube->mlx_win);
// 		free(cube->mlx_con);
// 		exit(1);
// 	}
//     return (0);
// }

// int handle_input_key_up(int keycode, t_cub *cube)
// {
//     if (keycode == 13 || keycode == 1)
//         cube->player->walk_direction = 0;
//     if (keycode == 2 || keycode == 0)
//         cube->player->strafe_direction = 0;
//     if (keycode == 124 || keycode == 123)
//         cube->player->turn_direction = 0;
//     return (0);
// }

// // int	ft_mouse_move(int x, int y, t_cub *cube)
// // {
// //     (void)cube;
// //     (void)y;
// //     if (x < cube->data->width && x > 0 && y < cube->data->height && y > 0)
// //     {
// //          static int last_x = -1;
// //         if (last_x == -1)
// //             last_x = x;
// //         int delta_x = x - last_x;
// //         last_x = x;
// //         cube->player->rotat_angle += delta_x * 0.03;  // Adjust sensitivity as needed
// //         cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
// //         // Redraw the scene with the updated player rotation
// //         draw_per(cube, 0);
// //     }
// //     return (1);
// // }

void my_keyhook(mlx_key_data_t keydata, void* param)
{
    (void)keydata;
    (void)param;
    t_cub * cube = param;
	if ((keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W) && keydata.action == MLX_PRESS)
		cube->player->walk_direction = 1;
	if ((keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S) && keydata.action == MLX_PRESS)
		cube->player->walk_direction = -1;

    if (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_PRESS)
		cube->player->turn_direction = 1;
	if (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_PRESS)
		cube->player->turn_direction = -1;
    
	if (((keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W) || (keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S)) && keydata.action == MLX_RELEASE)
		cube->player->walk_direction = 0;
	if ((keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT) && keydata.action == MLX_RELEASE)
		cube->player->turn_direction = 0;

    if (keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
		cube->player->strafe_direction = 1;
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
		cube->player->strafe_direction = -1;

    if ((keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A) && keydata.action == MLX_RELEASE)
		cube->player->strafe_direction = 0;

    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {
        mlx_close_window(cube->mlx);
        mlx_delete_image(cube->mlx, cube->image);
    }
}



void loop_fun(void* param)
{
    t_cub * cube = param;
    draw_all_black(cube);
    // draw_per(cube, 2);
    draw_map(cube);
    draw_per(cube);

}
// // end hooks
