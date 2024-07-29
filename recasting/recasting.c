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

void ft_get_player_pos(t_player *player , t_cub *cube)
{
    int x;
    int y;

    y = -1;
    while (++y < cube->data->map_row)
    {
        x = -1;
        while (++x < cube->data->map_cols)
        {
            if (cube->data->map[y][x] == cube->data->p)
            {
                player->player_x = (x * tile_size) + (tile_size / 2);
                player->player_y = (y * tile_size) + (tile_size / 2);
            }
        }
    }
}

t_player *init_player(t_cub *cube)
{
    t_player *player;

    player = (t_player *)malloc(sizeof(t_player));
    if (!player)
        malloc_error();
    ft_get_player_pos(player, cube);
    player->radius = 10;
    player->move_speed = 10;
    if (cube->data->p == 'W')
        player->rotat_angle = deg2rad(180);  // Initialize in radians
    else if (cube->data->p == 'S')
        player->rotat_angle = deg2rad(90);  // Initialize in radians
    else if (cube->data->p == 'N')
        player->rotat_angle = deg2rad(270);  // Initialize in radians
    else if (cube->data->p == 'E')
        player->rotat_angle = deg2rad(0);  // Initialize in radians
    player->rotation_speed = 0.09;
    player->turn_direction = 0;
    player->strafe_direction = 0;
    player->walk_direction = 0;
    player->start = 0;

    return (player);
}

int create_rgba(int r, int g, int b, int a)
{
    return (r << 24) | (g << 16) | (b << 8) | a;
}

void ft_fractol_init(t_cub *cube)
{
   cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();


    //check map
	// cube->image = mlx_new_image(cube->mlx, cube->data->width, cube->data->height);
	// if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, WIDTH / 10, HEIGHT / 10) < 0))
	// 	ft_error();


    //check 3D
    cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();


    // cube->mlx_con = mlx_init();
    // if (!cube->mlx_con)
    //     malloc_error();
    // cube->mlx_win = mlx_new_window(cube->mlx_con, WIDTH, HEIGHT, "Cub3D");
    // if (!cube->mlx_win)
    //     malloc_error();
    // cube->img.img_ptr = mlx_new_image(cube->mlx_con, WIDTH, HEIGHT);
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

    // Load and display textures
    int i = 0;
    while (i < 4)
    {
        cube->texture[i] = mlx_load_png("./wall.png");
        if (!cube->texture[i])
            ft_error();
        cube->img[i] = mlx_texture_to_image(cube->mlx, cube->texture[i]);
        if (!cube->img[i])
            ft_error();
        i++;
    }


    cube->player = init_player(cube);
}

void    draw_cube(t_cub *cube, int x, int y, int color)
{
	int	i;
	int	j;

	j = -1;
    while (++j < tile_size)
    {
        i = -1;
        while (++i < tile_size)
            mlx_put_pixel(cube->image,  (x * tile_size) + i ,  (y * tile_size) + j, color);
    }
    return ;
}

// all_black
void	draw_all_black(t_cub *cube)
{
	int	x;
	int	y;

	y = -1;
	x = -1;
	while (++y < HEIGHT)
	{
		x = -1;
		while (++x < WIDTH)
			mlx_put_pixel(cube->image, x, y, create_rgba(0, 0, 0, 150));
	}
}
// end all_black

// draw_player
int is_it_a_wall(double x, double y, t_cub *cube, int is)
{
    (void)is;
    double left = x - ((double)cube->player->radius);
    double up = y - ((double)cube->player->radius);
    double right = x + ((double)cube->player->radius);
    double down = y + ((double)cube->player->radius);

    if(left < 0 || right > cube->data->map_cols * tile_size || up < 0 || down > cube->data->map_row * tile_size)
        return (0);

    int  t_left = floor(left / tile_size);
    int  t_up = floor(up / tile_size);
    int  t_right = floor(right / tile_size);
    int  t_down = floor(down / tile_size);

    if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
        || cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
        return (0);
    return (1);
}

    // printf("%f %f\n", centerX , centerY);
void draw_player(t_cub *cube)
{
    double radius = (double)cube->player->radius;
    double centerX = cube->player->player_x;
    double centerY = cube->player->player_y;
    // printf("%f | %f | %f | %f | %f\n",cube->player->player_x, cube->player->player_y, cube->player->player_x, cube->player->player_y, radius);
    int y = -radius;
    while (y <= radius)
    {
        int x = -radius;
        y++;
        while (x <= radius)
        {
            if (x * x + y * y <= radius * radius)
                mlx_put_pixel(cube->image, centerX + (double)x , centerY + (double)y, RED);
                x++;
        }
    }
}

void DDA(t_cub *cube, double X0, double Y0, double X1, double Y1)
{
    double dx = X1 - X0;
    double dy = Y1 - Y0;
    double steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
    double Xinc = dx / steps;
    double Yinc = dy / steps;
    double X = X0;
    double Y = Y0;
    int i = 0;
    while (i <= steps)
    {
        mlx_put_pixel(cube->image, X , Y, create_rgba(250, 100, 100 , 255));
        X += Xinc;
        Y += Yinc;
        i++;
    }
}

int has_wall(t_cub *cube, double x1, double y1, int is)
{
    if (is == 1)
        y1--;
    else if (is == 2)
        x1--;
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

void ft_draw_hero(t_cub *cube, t_vars *vars)
{
    vars->y_intercept = floor(cube->player->player_y / tile_size) * tile_size;
    if (vars->isRayFacingDown)
        vars->y_intercept += tile_size;

    vars->x_intercept = cube->player->player_x + (vars->y_intercept - cube->player->player_y) / tan(vars->angle);

    vars->ystep = tile_size;
    if (vars->isRayFacingUp)
        vars->ystep *= -1;

    vars->xstep = tile_size / tan(vars->angle);
    if (vars->isRayFacingLeft && vars->xstep > 0)
        vars->xstep *= -1;
    if (vars->isRayFacingRight && vars->xstep < 0)
        vars->xstep *= -1;

    vars->nextHorzTouchX = vars->x_intercept;
    vars->nextHorzTouchY = vars->y_intercept;

    int a = 0;
    if (vars->isRayFacingUp)
        a = 1;

    while (vars->nextHorzTouchX > 0 && vars->nextHorzTouchX < cube->data->map_cols * tile_size &&
           vars->nextHorzTouchY > 0 && vars->nextHorzTouchY < cube->data->map_row * tile_size)
           {
        if (has_wall(cube, vars->nextHorzTouchX, vars->nextHorzTouchY, a))
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

    vars->x_intercept = floor(cube->player->player_x / tile_size) * tile_size;
    if (vars->isRayFacingRight)
        vars->x_intercept += tile_size;

    vars->y_intercept = cube->player->player_y + (vars->x_intercept - cube->player->player_x) * tan(vars->angle);

    vars->xstep = tile_size;
    if (vars->isRayFacingLeft)
        vars->xstep *= -1;

    vars->ystep = tile_size * tan(vars->angle);
    if (vars->isRayFacingUp && vars->ystep > 0)
        vars->ystep *= -1;
    if (vars->isRayFacingDown && vars->ystep < 0)
        vars->ystep *= -1;

    vars->nextVertTouchX = vars->x_intercept;
    vars->nextVertTouchY = vars->y_intercept;

    int a = 0;
    if (vars->isRayFacingLeft)
        a = 2;

        // printf("%f | %f | %f | %f\n", vars->nextVertTouchX, vars->nextVertTouchY, cube->player->player_x, cube->player->player_y);
    while (vars->nextVertTouchX > 0 && vars->nextVertTouchX < cube->data->map_cols * tile_size &&
           vars->nextVertTouchY > 0 && vars->nextVertTouchY < cube->data->map_row * tile_size)
           {
        if (has_wall(cube, vars->nextVertTouchX, vars->nextVertTouchY, a))
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

    vars.isRayFacingDown = vars.angle > 0 && vars.angle < M_PI;
    vars.isRayFacingUp = !vars.isRayFacingDown;
    vars.isRayFacingRight = vars.angle < 0.5 * M_PI || vars.angle > 1.5 * M_PI;
    vars.isRayFacingLeft = !vars.isRayFacingRight;

    vars.foundHorzWallHit = 0;
    vars.horzWallHitX = 0;
    vars.horzWallHitY = 0;

    ft_draw_hero(cube, &vars);
    ft_draw_ver(cube, &vars);
    vars.horzHitDistance = __INT_MAX__;
    vars.vertHitDistance = __INT_MAX__;

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

    if (is == 1)
        DDA(cube, cube->player->player_x, cube->player->player_y, vars.wallHitX, vars.wallHitY);

    return vars;
}

void draw_lines(t_cub *cube, int is)
{
    cube->is = MAP_SCALE;
    double angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
    int i = 0;
    while (i < NUM_RAYS)
    {
        draw_line(cube, angle, is);
        angle -= FOV_ANGLE / NUM_RAYS;
        i++;
    }
}

uint32_t ft_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 24) | (g << 16) | (b << 8) | 0xFF;
}

uint32_t get_pixel_color(mlx_texture_t* texture, int x, int y)
{
    if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
        return 0;
    int index = (y * texture->width + x) * 4; // 4 bytes per pixel
    uint8_t* pixels = texture->pixels;
    uint32_t color = *(uint32_t*)&pixels[index];

    // Assuming the color format is ARGB and we need RGBA
    uint8_t a = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // Convert ARGB to RGBA
    return (r << 24) | (g << 16) | (b << 8) | a;
}

void draw_lines_3D(t_cub* cube)
{
    double angle;
    double distanceProjPlane = ((double)WIDTH / 2.0) / tan(FOV_ANGLE / 2);
    // double rayWidth = (double)WIDTH / NUM_RAYS;

    angle = cube->player->rotat_angle - FOV_ANGLE / 2.0;

    // Draw sky and floor
    int i = 0;
    while (i < WIDTH)
    {
        int j = 0;
        while (j < HEIGHT / 2)
        {
            mlx_put_pixel(cube->image, i, j, create_rgba(cube->data->sky.r, cube->data->sky.g, cube->data->sky.b, 255));
            j++;
        }
        j = HEIGHT / 2;
        while (j < HEIGHT)
        {
            mlx_put_pixel(cube->image, i, j, create_rgba(cube->data->floor.r, cube->data->floor.g, cube->data->floor.b, 255));
            j++;
        }
        i++;
    }
    i = 0;
    while (i < WIDTH)
    {
        t_vars vars = draw_line(cube, angle, 0);

        double wallDistance = vars.distance * cos(angle - cube->player->rotat_angle);
        double projectedWallHeight = (tile_size / wallDistance) * distanceProjPlane;

        double wallStripHeight = projectedWallHeight;
        double wallTopPixel = ((double)HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
        double wallBottomPixel = ((double)HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > (double)HEIGHT ? (double)HEIGHT : wallBottomPixel;

        double textureStep = (double)cube->texture[0]->height / wallStripHeight;
        double textureOffsetY = 0;

        if (wallStripHeight > HEIGHT)
        {
            textureOffsetY = ((double)wallStripHeight - (double)HEIGHT) / 2.0;
            wallStripHeight = HEIGHT;
        }

        int textureNum = 0;
        if (vars.wasHitVert)
        {
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

        mlx_texture_t* texture = cube->texture[textureNum];
        int textureX;
        if (vars.wasHitVert)
            textureX = (int)vars.wallHitY % tile_size;
        else
            textureX = (int)vars.wallHitX % tile_size;

        double texturePos = textureOffsetY * textureStep;

        // int x = i * rayWidth;
        int y = wallTopPixel;
        while (y < wallBottomPixel)
        {
            int textureY = (int)texturePos & (texture->height - 1);
            texturePos += textureStep;

            uint32_t color = get_pixel_color(texture, textureX, textureY);
            mlx_put_pixel(cube->image, i, y, color);
            // double shadeFactor = 10 / (1.0 + vars.distance * 0.1);
            // uint8_t r = ((color >> 24) & 0xFF) * shadeFactor;
            // uint8_t g = ((color >> 16) & 0xFF) * shadeFactor;
            // uint8_t b = ((color >> 8) & 0xFF) * shadeFactor;
            // uint32_t shadedColor = (r << 24) | (g << 16) | (b << 8) | 0xFF;
            // mlx_put_pixel(cube->image, i, y, shadedColor);
            y++;
        }
        angle += FOV_ANGLE / WIDTH;
        i++;
    }
}

int create_rgb_color(int r, int g, int b)
{
    return (r << 16) | (g << 8) | b;
}

void draw_view_player(t_cub *cube, int is)
{
    // Normalize the player's rotation angle
    int i = cube->player->move_speed;
    cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
    cube->player->rotat_angle += (double)cube->player->turn_direction  * cube->player->rotation_speed;
    while (i--)
    {

        // Calculate movement step
        double movestep = (double)cube->player->walk_direction * i;
        double new_player_x;
        double new_player_y;
        (void)is;
        new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
        new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

        if (cube->player->strafe_direction != 0)
        {
            new_player_x += (double)cube->player->strafe_direction * (i / 2) * cos(cube->player->rotat_angle + M_PI / 2);
            new_player_y += (double)cube->player->strafe_direction * (i / 2) * sin(cube->player->rotat_angle + M_PI / 2);
        }

        if (is_it_a_wall(new_player_x, new_player_y, cube, is))
        {
            cube->player->player_x = new_player_x;
            cube->player->player_y = new_player_y;
            break;
        }
        if (is_it_a_wall(cube->player->player_x, new_player_y, cube, is))
        {
            cube->player->player_y = new_player_y;
            break;
        }
        if (is_it_a_wall(new_player_x, cube->player->player_y, cube, is))
        {
            cube->player->player_x = new_player_x;
            break;
        }
    }
    
    // cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
    // cube->player->rotat_angle += (double)cube->player->turn_direction  * cube->player->rotation_speed;
    // // Calculate movement step
    // double movestep = (double)cube->player->walk_direction * cube->player->move_speed;
    // double new_player_x;
    // double new_player_y;
    // (void)is;
    // new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
    // new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);
    // // Handle strafe movement (left/right)
    // if (cube->player->strafe_direction != 0)
    // {
    //     new_player_x += (double)cube->player->strafe_direction * (cube->player->move_speed / 2) * cos(cube->player->rotat_angle + M_PI / 2);
    //     new_player_y += (double)cube->player->strafe_direction * (cube->player->move_speed / 2) * sin(cube->player->rotat_angle + M_PI / 2);
    // }
    // // Check if the new position is within bounds and not a wall
    // if (is_it_a_wall(new_player_x, new_player_y, cube, is))
    // {
    //     cube->player->player_x = new_player_x;
    //     cube->player->player_y = new_player_y;
    // }
    // else
    // {
    //     // Try sliding along the wall
    //     if (is_it_a_wall(cube->player->player_x, new_player_y, cube, is))
    //         cube->player->player_y = new_player_y;
    //     else if (is_it_a_wall(new_player_x, cube->player->player_y, cube, is))
    //         cube->player->player_x = new_player_x;
    // }

    draw_lines_3D(cube);
}

void	handle_pixel2(int x, int y, t_cub *cube)
{
    if(cube->data->map[y][x] == cube->data->p)
    {
        // draw_cube(cube, x, y, create_rgba(0, 255, 255, 255));
        draw_view_player(cube, 1);
        // draw_player(cube);
        // draw_lines(cube, 1);
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
//end draw_player

// draw_map
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
// end draw_map

// hooks
void my_keyhook(mlx_key_data_t keydata, void* param)
{
    (void)keydata;
    (void)param;
    t_cub * cube = param;

    if(keydata.action == MLX_PRESS)
    {
        if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W)
            cube->player->walk_direction = 1;
        if ((keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S))
            cube->player->walk_direction = -1;

        if (keydata.key == MLX_KEY_RIGHT)
            cube->player->turn_direction = 1;
        if (keydata.key == MLX_KEY_LEFT)
            cube->player->turn_direction = -1;

        if (keydata.key == MLX_KEY_D)
            cube->player->strafe_direction = 1;
        if (keydata.key == MLX_KEY_A)
            cube->player->strafe_direction = -1;

        if (keydata.key == MLX_KEY_ESCAPE)
        {
            mlx_close_window(cube->mlx);
            mlx_delete_image(cube->mlx, cube->image);
        }
        if(keydata.key == MLX_KEY_LEFT_SHIFT)
            cube->player->move_speed = 5;
    }

    if(keydata.action == MLX_RELEASE)
    {
        if (keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_DOWN || keydata.key == MLX_KEY_S)
            cube->player->walk_direction = 0;
        if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
            cube->player->turn_direction = 0;

        if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A)
            cube->player->strafe_direction = 0;
        
        if(keydata.key == MLX_KEY_LEFT_SHIFT)
            cube->player->move_speed = 10;
    }

}

void    handle_mouse(t_cub *cube)
{
    int32_t prev_xpos = WIDTH / 2;
    int32_t xpos, ypos;
    double sensitivity = 0.002;

    mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

    int32_t delta_x = xpos - prev_xpos;

    // printf("%d | %d | %d | %d | %f\n",prev_xpos, xpos, ypos, delta_x, cube->player->rotat_angle);
    cube->player->rotat_angle += delta_x * sensitivity;
    mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);

    mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
}

void fill_rectangle(t_cub* cube, int x, int y, int width, int height, int color)
{
    int j = y;
    while (j < y + height)
    {
        int i = x;
        while (i < x + width)
        {
            mlx_put_pixel(cube->image, i, j, color);
            i++;
        }
        j++;
    }
}

void loop_fun(void* param)
{
    t_cub* cube = (t_cub*)param;
    int32_t xpos, ypos;
    mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

    if(xpos != WIDTH / 2 && ypos != HEIGHT / 2 && cube->player->start == 0)
    {
        mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
        cube->player->start = 1;
    }
    else
        handle_mouse(cube);
    draw_all_black(cube);
    // draw_map(cube);
    draw_per(cube);
    fill_rectangle(cube, MINIMAP_X_OFFSET, MINIMAP_Y_OFFSET, MINIMAP_SIZE, MINIMAP_SIZE, create_rgba(0, 0, 0, 128));
}
// // end hooks
