#include "../../recasting.h"

static void ft_error(void)
{
	fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void    ft_free_data(t_cub *cube)
{
	free_double_arr(cube->data->map);
	free(cube->data->ea);
	free(cube->data->so);
	free(cube->data->no);
	free(cube->data->we);
	free(cube->player);
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

	player = (t_player *)ft_calloc(sizeof(t_player));
	if (!player)
		malloc_error();
	ft_get_player_pos(player, cube);
	player->radius = 10;
	player->move_speed = 4;
	if (cube->data->p == 'W')
		player->rotat_angle = deg2rad(180);  // Initialize in radians
	else if (cube->data->p == 'S')
		player->rotat_angle = deg2rad(90);  // Initialize in radians
	else if (cube->data->p == 'N')
		player->rotat_angle = deg2rad(270);  // Initialize in radians
	else if (cube->data->p == 'E')
		player->rotat_angle = deg2rad(0);  // Initialize in radians
	player->rotation_speed = 0.04;
	player->turn_direction = 0;
	player->strafe_direction = 0;
	player->walk_direction = 0;
	player->player_z = 0;
	player->start = 0;
	player->jump = 0;
	player->tab = 0;
	player->jump_var = 0;
	player->middle = 0;
	player->mouse = 0;
	player->stop_mouse = 0;
	player->right_left = 0;
	return (player);
}

int create_rgba(int r, int g, int b, int a)
{
	return (r << 24) | (g << 16) | (b << 8) | a;
}

void    ft_load_doors(t_cub *cube, int i, char *path)
{
	cube->doors[i] = mlx_load_png(path);
	if (!cube->doors[i])
		ft_error();
	cube->door_img[i] = mlx_texture_to_image(cube->mlx, cube->doors[i]);
	if (!cube->door_img[i])
		ft_error();
}

void ft_fractol_init(t_cub *cube)
{
   cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();

	// Load and display textures
	int i = 0;
	cube->gun[0] = mlx_load_png("../Textures/png/call-of-duty-wiki-call-of-duty-modern-warfare-machine-gun-weapon-weaponry-armory-transparent-png-1324476.png");
	if (!cube->gun[0])
		ft_error();
	cube->gun_img[0] = mlx_texture_to_image(cube->mlx, cube->gun[0]);
	if (!cube->gun_img[0])
		ft_error();
	//door
	ft_load_doors(cube, 0, "../Textures/Doors/tile000.png");
	ft_load_doors(cube, 1, "../Textures/Doors/tile001.png");
	ft_load_doors(cube, 2, "../Textures/Doors/tile002.png");
	ft_load_doors(cube, 3, "../Textures/Doors/tile003.png");
	ft_load_doors(cube, 4, "../Textures/Doors/tile004.png");
	ft_load_doors(cube, 4, "../Textures/Doors/tile004.png");
	ft_load_doors(cube, 5, "../Textures/Doors/tile005.png");
	ft_load_doors(cube, 6, "../Textures/Doors/tile006.png");
	ft_load_doors(cube, 7, "../Textures/Doors/tile007.png");
	ft_load_doors(cube, 8, "../Textures/Doors/tile008.png");
	char *texture_files[] = {cube->data->no, cube->data->so, cube->data->we, cube->data->ea,\
	"../Textures/Health/tile000.png", "../Textures/Health/tile001.png", "../Textures/Health/tile002.png",\
	"../Textures/Health/tile003.png", "../Textures/Health/tile004.png"};
	while (i < 9)
	{
		cube->texture[i] = mlx_load_png(texture_files[i]);
		if (!cube->texture[i])
			ft_error();
		cube->img[i] = mlx_texture_to_image(cube->mlx, cube->texture[i]);
		if (!cube->img[i])
			ft_error();
		i++;
	}
	cube->player = init_player(cube);
	if (mlx_image_to_window(cube->mlx, cube->gun_img[0], WIDTH/ 2, HEIGHT - cube->gun[0]->height) < 0)
		ft_error();
	if (mlx_image_to_window(cube->mlx, cube->img[4], 20 , HEIGHT - cube->img[4]->height - 20) < 0)
		ft_error();
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

t_pos ft_is_a_door(double x, double y, t_cub *cube)
{
	(void)x;
	(void)y;
	t_pos p;

	p.x = -1;
	p.y = -1;
	while (++p.x < cube->data->map_row)
	{
		p.y = -1;
		while (++p.y < cube->data->map_row)
		{
			if (p.x <= cube->data->map_cols && cube->data->map[p.x][p.y] == 'D')
				return (p);
		}
	}
	return (p);
}

int is_it_a_wall(double x, double y, t_cub *cube)
{
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

// void	ft_draw_line(t_point x1, t_point x2, t_fdf *box)
// {
// 	box->draw.dx = abs(x2.x - x1.x);
// 	box->draw.dy = abs(x2.y - x1.y);
// 	if (x1.x < x2.x)
// 		box->draw.sx = 1;
// 	else
// 		box->draw.sx = -1;
// 	if (x1.y < x2.y)
// 		box->draw.sy = 1;
// 	else
// 		box->draw.sy = -1;
// 	box->draw.error = box->draw.dx - box->draw.dy;
// 	while (1)
// 	{
// 		if (x1.x >= 0 && x1.y >= 0 && x1.x < WIDTH && x1.y < HEIGHT)
// 			my_pixel_put(x1.x, x1.y, box->var.color, box);
// 		if (x1.x == x2.x && x1.y == x2.y)
// 			break ;
// 		box->draw.e2 = 2 * box->draw.error;
// 		if (box->draw.e2 >= -box->draw.dy)
// 		{
// 			box->draw.error -= box->draw.dy;
// 			x1.x += box->draw.sx;
// 		}
// 		if (box->draw.e2 <= box->draw.dx)
// 		{
// 			box->draw.error += box->draw.dx;
// 			x1.y += box->draw.sy;
// 		}
// 	}
// }

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

	if (cube->data->map[y][x] == '1' || cube->data->map[y][x] == 'D')
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

	// Read individual color components
	uint8_t r = pixels[index];
	uint8_t g = pixels[index + 1];
	uint8_t b = pixels[index + 2];
	uint8_t a = pixels[index + 3];

	// Combine into RGBA format
	return (r << 24) | (g << 16) | (b << 8) | a;
}

// 3d

void ft_draw_sky_floor(t_cub *cube)
{
	int i, j;
	int sky_end_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;
	int floor_start_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;

	if (sky_end_y < 0)
		sky_end_y = 0;
	if (floor_start_y >= HEIGHT)
		floor_start_y = HEIGHT - 1;

	// Draw sky and floor
	i = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			if (j < sky_end_y)
				mlx_put_pixel(cube->image, i, j, create_rgba(cube->data->sky.r, cube->data->sky.g, cube->data->sky.b, 255));
			else if (j >= floor_start_y)
				mlx_put_pixel(cube->image, i, j, create_rgba(cube->data->floor.r, cube->data->floor.g, cube->data->floor.b, 255));
			++j;
		}
		++i;
	}
}

void ft_get_texture(t_cub *cube, t_vars vars, int textureNum, int i, int door)
{
    mlx_texture_t* texture = vars.door ? cube->doors[door] : cube->texture[textureNum];

    double texturePosX = vars.wasHitVert ?
        fmod(vars.wallHitY, tile_size) / tile_size :
        fmod(vars.wallHitX, tile_size) / tile_size;
    texturePosX = 1.0 - texturePosX;

    int textureX = (int)(texturePosX * texture->width);
    double texturePos = vars.textureOffsetY * vars.textureStep;
    double shade = fmax(0.3, 1.0 - (vars.distance / 1000.0));

    for (int y = vars.wallTopPixel; y < vars.wallBottomPixel && y < HEIGHT; y++)
    {
        int textureY = (int)(texturePos * texture->height) % texture->height;
        texturePos += vars.textureStep;

        uint32_t color = get_pixel_color(texture, textureX, textureY);

        uint8_t r = ((color >> 24) & 0xFF) * shade;
        uint8_t g = ((color >> 16) & 0xFF) * shade;
        uint8_t b = ((color >> 8) & 0xFF) * shade;
        uint32_t shaded_color = (r << 24) | (g << 16) | (b << 8) | (color & 0xFF);

        if (!(vars.door && ((shaded_color & 0xFFFFFF00) == 0)) && y >= 0)
        {
            mlx_put_pixel(cube->image, i, y, shaded_color);
        }
    }
}

// Add this function to your code
void draw_textured_floor(t_cub *cube)
{
	// Player's direction
	double dir_x = cos(cube->player->rotat_angle);
	double dir_y = sin(cube->player->rotat_angle);

	// Plane vector perpendicular to the direction vector
	double plane_x = -dir_y;
	double plane_y = dir_x;

	for (int y = HEIGHT / 2 + 1; y < HEIGHT; y++)
	{
		int p = y - (HEIGHT / 2 + cube->player->player_z + cube->player->jump_var);

		double pos_z = 0.5 * HEIGHT;

		double row_distance = pos_z / p;

		double step_x = row_distance * (dir_x + plane_x) / WIDTH;
		double step_y = row_distance * (dir_y + plane_y) / WIDTH;

		double floor_x = cube->player->player_x + row_distance * (dir_x - plane_x);
		double floor_y = cube->player->player_y + row_distance * (dir_y - plane_y);

		for(int x = 0; x < WIDTH; ++x)
		{
			int cell_x = (int)(floor_x);
			int cell_y = (int)(floor_y);

			int tx = (int)(cube->texture[0]->width * (floor_x - cell_x)) & (cube->texture[0]->width - 1);
			int ty = (int)(cube->texture[0]->height * (floor_y - cell_y)) & (cube->texture[0]->height - 1);

			floor_x += step_x;
			floor_y += step_y;

			uint32_t color = get_pixel_color(cube->texture[0], tx, ty);
			mlx_put_pixel(cube->image, x, y, color);
		}
	}
}

void draw_lines_3D(t_cub* cube)
{
    double distanceProjPlane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
    double angle = cube->player->rotat_angle - FOV_ANGLE / 2.0;
    double angleStep = FOV_ANGLE / WIDTH;

    ft_draw_sky_floor(cube);

    for (int i = 0; i < WIDTH; i++)
    {
        t_vars vars = draw_line(cube, angle, 0);

        double wallDistance = vars.distance * cos(angle - cube->player->rotat_angle);
        double wallStripHeight = (tile_size / wallDistance) * distanceProjPlane;

        vars.wallTopPixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var;
        vars.wallBottomPixel = fmin((HEIGHT / 2.0) + (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var, HEIGHT);

        vars.textureStep = 1.0 / wallStripHeight;
        vars.textureOffsetY = 0;

        int textureNum = vars.wasHitVert ?
            (vars.isRayFacingLeft ? 2 : 3) :
            (vars.isRayFacingUp ? 0 : 1);

        ft_get_texture(cube, vars, textureNum, i, cube->doortype);
        angle += angleStep;
    }
}

// hooks
void my_keyhook(mlx_key_data_t keydata, void* param)
{
	(void)keydata;
	(void)param;
	t_cub * cube = param;
	if(keydata.action == MLX_PRESS)
	{

		// if (keydata.key == MLX_KEY_RIGHT_SHIFT)
		//     cube->player->stop_mouse = 1;

		if (keydata.key == MLX_KEY_T)
			cube->player->right_left = 1;

		if (keydata.key == MLX_KEY_RIGHT_SHIFT)
			cube->player->mouse = 1;

		if (keydata.key == MLX_KEY_TAB)
			cube->player->tab = 1;

		if (keydata.key == MLX_KEY_RIGHT)
			cube->player->turn_direction = 1;
		if (keydata.key == MLX_KEY_LEFT)
			cube->player->turn_direction = -1;

		if (keydata.key == MLX_KEY_W)
			cube->player->walk_direction = 1;
		if (keydata.key == MLX_KEY_UP)
			cube->player->walk_direction = 1;
		if (keydata.key == MLX_KEY_S)
			cube->player->walk_direction = -1;
		if (keydata.key == MLX_KEY_DOWN)
			cube->player->walk_direction = -1;

		if (keydata.key == MLX_KEY_D)
			cube->player->strafe_direction = 1;
		if (keydata.key == MLX_KEY_A)
			cube->player->strafe_direction = -1;

		if (keydata.key == MLX_KEY_ESCAPE)
		{
			ft_free_data(cube);
			mlx_close_window(cube->mlx);
			mlx_delete_image(cube->mlx, cube->image);
		}

		if(keydata.key == MLX_KEY_LEFT_SHIFT)
			cube->player->jump = 2;
		if(keydata.key == MLX_KEY_SPACE)
			cube->player->jump = 1;
		if(keydata.key == MLX_KEY_LEFT_CONTROL)
			cube->player->jump = -1;

		if(keydata.key == MLX_KEY_F)
		{
			// mlx_delete_image(cube->mlx, cube->image);
			// cube->mlx = mlx_init(3000, 2000, "42Balls", true);
			// if (!cube->mlx)
			//     ft_error();

			// cube->image = mlx_new_image(cube->mlx, 3000, 2000);
			// if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
			//     ft_error();
		}
	}

	if(keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_DOWN)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_S)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_UP)
			cube->player->walk_direction = 0;
		if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
			cube->player->turn_direction = 0;

		if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A)
			cube->player->strafe_direction = 0;

		if(keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_LEFT_CONTROL || keydata.key == MLX_KEY_LEFT_SHIFT)
			cube->player->jump = 0;
		if(keydata.key == MLX_KEY_F)
		{
		}
		// if(keydata.key == MLX_KEY_UP || keydata.key == MLX_KEY_DOWN)
		//     cube->player->is_moving_up = 0;

		if (keydata.key == MLX_KEY_TAB)
			cube->player->tab = 0;

		if (keydata.key == MLX_KEY_T)
			cube->player->right_left = 0;

	}

}

void handle_mouse(t_cub *cube)
{
	static int32_t prev_xpos = WIDTH / 2;
	static int32_t prev_ypos = HEIGHT / 2;
	int32_t xpos, ypos;
	double sensitivity = 0.001;

	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	if (xpos < 0 || xpos > WIDTH || ypos < 0 || ypos > HEIGHT)
		return ;

	int32_t delta_x = xpos - prev_xpos;
	int32_t delta_y = ypos - prev_ypos;

	cube->player->rotat_angle += delta_x * sensitivity;

	mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);

	prev_xpos = WIDTH / 2;
	prev_ypos = HEIGHT / 2;

	mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
	int z = cube->player->player_z + delta_y;
	if (z < 500 && z > -500)
		cube->player->player_z = z;
}

void update_player(t_cub *cube)
{
	static int door;
	static int last_step;
	if (cube->player->jump == 1)
		cube->player->jump_var = -100;
	else if (cube->player->jump == -1)
	{
		cube->player->move_speed = 3;
		cube->player->jump_var = 100;
	}
	else if (cube->player->jump == 0)
	{
		cube->player->move_speed = 4;
		cube->player->jump_var = 0;
	}
	else if (cube->player->jump == 2)
		cube->player->move_speed = 2;

	int move_speed = cube->player->move_speed;
	cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
	cube->player->rotat_angle += (double)cube->player->turn_direction * cube->player->rotation_speed;

	while (move_speed--) {
		// Calculate movement step
		double movestep = (double)cube->player->walk_direction * move_speed;
		double new_player_x;
		double new_player_y;
		new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
		new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

		if (cube->player->strafe_direction != 0) {
			new_player_x += (double)cube->player->strafe_direction * (move_speed / 1.5) * cos(cube->player->rotat_angle + M_PI / 2);
			new_player_y += (double)cube->player->strafe_direction * (move_speed / 1.5) * sin(cube->player->rotat_angle + M_PI / 2);
		}
		t_pos pos = ft_is_a_door(new_player_x, new_player_y, cube);
		// printf("[%d %d] %d %d\n",(int)floor(cube->player->player_x / tile_size), (int)floor(cube->player->player_y / tile_size) ,pos.x, pos.y);
		if (pos.x > 0 && pos.y  > 0)
		{
			if (floor(cube->player->player_x / tile_size) > (pos.y - 6))
			{
				if (door != 8 && last_step != floor(cube->player->player_x / tile_size))
				{
					last_step = floor(cube->player->player_x / tile_size);
					door += 2;
				}
			}
			else
			{
				if (door != 0)
				{
					last_step = 0;
					door--;
				}
			}
		}
		// Check for wall collision before updating player position
		if (is_it_a_wall(new_player_x, new_player_y, cube)) {
			// No collision, update both x and y
			cube->player->player_x = new_player_x;
			cube->player->player_y = new_player_y;
		} else {
			// Check for sliding along the wall
			if (is_it_a_wall(cube->player->player_x, new_player_y, cube)) {
				// No collision in y direction, update y only
				cube->player->player_y = new_player_y;
			}
			if (is_it_a_wall(new_player_x, cube->player->player_y, cube)) {
				// No collision in x direction, update x only
				cube->player->player_x = new_player_x;
			}
		}
	}
	cube->doortype = door;
}

// heal
void    draw_inside_head(t_cub *cube)
{
	int i = 0;
	int y;
	while (i <= 400)
	{
		y = 0;
		while (y <= 20)
		{
			if(i != 0 && y != 0 && y != 20 && i != 400)
				mlx_put_pixel(cube->image, i + 10, y + 10, create_rgba(0, 0, 255, 255));
			y++;
		}
		i++;
	}
}

void    heal_bar(t_cub *cube)
{
	int i = 0;
	int y;
	while (i <= 400)
	{
		y = 0;
		while (y <= 20)
		{
			if(i == 0 || y == 0 || y == 20 || i == 400)
				mlx_put_pixel(cube->image, i + 10, y + 10, create_rgba(255, 0, 0, 255));
			y++;
		}
		i++;
	}
	draw_inside_head(cube);
}
// end heal

// shots
void    draw_shots(t_cub *cube)
{
	int i = 0;
	int y;
	while (i <= 280)
	{
		y = 0;
		while (y <= 150)
		{
			if(i == 135)
				mlx_put_pixel(cube->image, i + WIDTH / 1.2, y + HEIGHT / 1.2, create_rgba(0, 0, 255, 255));
			else
				mlx_put_pixel(cube->image, i + WIDTH / 1.2, y + HEIGHT / 1.2, create_rgba(0, 0, 0, 255));
			y++;
		}
		i++;
	}
}
// end shots

void loop_fun(void* param)
{
    t_cub* cube = (t_cub*)param;
    update_player(cube);

    draw_lines_3D(cube);
    draw_per(cube);
    ft_draw_player(cube, WIDTH / 2, HEIGHT / 2);

    // Mouse handling
    int32_t xpos, ypos;
    mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

    if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && cube->player->start == 0)
    {
        mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
        cube->player->start = 1;
    }
    else if (!mlx_is_key_down(cube->mlx, MLX_KEY_RIGHT_SHIFT))
        handle_mouse(cube);
    else
        mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
// // end hooks