#include "../../recasting.h"

void	ft_error(void)
{
	fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	ft_free_data(t_cub *cube)
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
	return (degrees * (M_PI / 180.0));
}

void	ft_get_player_pos(t_player *player, t_cub *cube)
{
	int		x;
	int		y;
	t_doors	*head;
	t_doors	*new;

	y = -1;
	head = NULL;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
		{
			if (cube->data->map[y][x] == 'D' || cube->data->map[y][x] == 'O')
			{
				new = ft_lstnew_doors(x, y);
				ft_lstaddback_doors(&head, new);
			}
			if (cube->data->map[y][x] == cube->data->p)
			{
				player->player_x = (x * tile_size) + (tile_size / 2);
				player->player_y = (y * tile_size) + (tile_size / 2);
			}
		}
	}
	cube->doors_locations = head;
}

void	init_player_1(t_cub *cube, t_player *player)
{
	player->minimap = 1;
	player->tab = 0;
	player->jump_var = 0;
	player->middle = 0;
	player->mouse = 0;
	player->stop_mouse = 0;
	player->right_left = 0;
	cube->current_gun_index = 0;
	cube->cur_g_right_clikc = 100;
	cube->y_press = 0;
	cube->t_press = 0;
	player->shift = 1;
	player->open = 1;
	cube->right_press = 0;
}

t_player	*init_player(t_cub *cube)
{
	t_player	*player;

	player = (t_player *)ft_calloc(sizeof(t_player));
	if (!player)
		malloc_error();
	ft_get_player_pos(player, cube);
	player->radius = 10;
	player->move_speed = 0;
	if (cube->data->p == 'W')
		player->rotat_angle = deg2rad(180);
	else if (cube->data->p == 'S')
		player->rotat_angle = deg2rad(90);
	else if (cube->data->p == 'N')
		player->rotat_angle = deg2rad(270);
	else if (cube->data->p == 'E')
		player->rotat_angle = deg2rad(0);
	player->rotation_speed = 0.04;
	player->turn_direction = 0;
	player->strafe_direction = 0;
	player->walk_direction = 0;
	player->player_z = 0;
	player->start = 0;
	player->jump = 0;
	init_player_1(cube, player);
	return (player);
}

int	c_rgba(int r, int g, int b, int a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

static char	*zero(void)
{
	char	*arr;

	arr = malloc(2 * sizeof(char));
	if (!arr)
		return (NULL);
	if (arr == NULL)
		return (NULL);
	arr[0] = '0';
	arr[1] = '\0';
	return (arr);
}

static int	ft_intlen(int n)
{
	int	nbdg;
	int	tmp;

	nbdg = 0;
	tmp = n;
	while (tmp != 0)
	{
		tmp /= 10;
		nbdg++;
	}
	return (nbdg);
}

static void	fillarr(char *res, int n, int nbrdg, int isnegative)
{
	int	index;
	int	digit;

	index = nbrdg - 1;
	while (n != 0)
	{
		if (n < 0)
			digit = -(n % 10);
		else
			digit = n % 10;
		res[index] = digit + '0';
		n /= 10;
		index--;
	}
	if (isnegative != 0)
		res[0] = '-';
	res[nbrdg] = '\0';
}

char	*ft_itoa(int n)
{
	int		nbrdg;
	int		isnegative;
	char	*res;

	if (n == 0)
		return (zero());
	nbrdg = ft_intlen(n);
	isnegative = 0;
	if (n < 0)
	{
		isnegative = 1;
		nbrdg++;
	}
	res = malloc((nbrdg + 1) * sizeof(char));
	if (!res)
		return (NULL);
	fillarr(res, n, nbrdg, isnegative);
	return (res);
}

void	ft_load_doors(t_cub *cube, int i, char *path)
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
	char **guns = (char **)ft_calloc(sizeof(char *) * (Y_CLICK + R_CLICK + 1));
	if (!guns)
		return ;
	int i = 0;
	while (i < Y_CLICK + R_CLICK)
	{
		char *tmp = ft_strdup("../Textures/png/guns_0/");
		char *num = ft_itoa(i);
		char *str = ft_strjoin(tmp, num);
		str = ft_strjoin(str, ".png");
		guns[i] = str;
		i++;
		free(num);
	}
	guns[i] = NULL;

	i = 0;
	while (i < Y_CLICK)
	{
		cube->gun[i] = mlx_load_png(guns[i]);
		if (!cube->gun[i])
			ft_error();
		if (i < 2)
		{
			cube->gun_img[i] = mlx_texture_to_image(cube->mlx, cube->gun[i]);
			if (!cube->gun_img[i])
				ft_error();
		}
		i++;
	}

	int k = 0;
	while (guns[i])
	{
		cube->gun_r[k] = mlx_load_png(guns[i]);
		if (!cube->gun_r[k])
			ft_error();
		if (k < 2)
		{
			cube->gun_r_img[k] = mlx_texture_to_image(cube->mlx, cube->gun_r[k]);
			if (!cube->gun_r_img[k])
				ft_error();
		}
		k++;
		i++;
	}

	free_double_arr(guns);

	ft_load_doors(cube, 0, "../Textures/Doors/tile000.png");
	char *texture_files[] = {cube->data->no, cube->data->so, cube->data->we, cube->data->ea};
	i = 0;
	while (i < 4)
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
			mlx_put_pixel(cube->image, (x * tile_size) + i,
				(y * tile_size) + j, color);
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
			mlx_put_pixel(cube->image, x, y, c_rgba(0, 0, 0, 150));
	}
}
// end all_black

// draw_player
// t_pos ft_is_a_door(double x, double y, t_cub *cube)
// {
// 	(void)x;
// 	(void)y;
// 	t_pos p;
// 	p.x = -1;
// 	p.y = -1;
// 	while (++p.x < cube->data->map_row)
// 	{
// 		p.y = -1;
// 		while (++p.y < cube->data->map_row)
// 		{
// 			if (p.x <= cube->data->map_cols && (cube->data->map[p.x][p.y] == 'D' || cube->data->map[p.x][p.y] == 'T'))
// 				return (p);
// 		}
// 	}
// 	return (p);
// }

int	is_it_a_wall(double x, double y, t_cub *cube)
{
	double left;
	double up;
	double right;
	double down;
	int  t_left;
	int  t_up;
	int  t_right;
	int  t_down;

	left = x - ((double)cube->player->radius);
	up = y - ((double)cube->player->radius);
	right = x + ((double)cube->player->radius);
	down = y + ((double)cube->player->radius);
	if (left < 0 || right > cube->data->map_cols * tile_size
		|| up < 0 || down > cube->data->map_row * tile_size)
		return (0);
	t_left = floor(left / tile_size);
	t_up = floor(up / tile_size);
	t_right = floor(right / tile_size);
	t_down = floor(down / tile_size);
	if (cube->data->map[t_up][t_left] == '1' || cube->data->map[t_down][t_right] == '1'
		|| cube->data->map[t_up][t_right] == '1' || cube->data->map[t_down][t_left] == '1')
		return (0);
	if ((cube->data->map[t_up][t_left] == 'D' || cube->data->map[t_down][t_right] == 'D'
		|| cube->data->map[t_up][t_right] == 'D' || cube->data->map[t_down][t_left] == 'D') && cube->player->open == 1)
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

void	DDA(t_cub *cube, double X0, double Y0, double X1, double Y1)
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
		mlx_put_pixel(cube->image, X , Y, c_rgba(250, 100, 100 , 255));
		X += Xinc;
		Y += Yinc;
		i++;
	}
}

double	normalizeAngle(double angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}

uint32_t	ft_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

uint32_t	get_pixel_color(mlx_texture_t* texture, int x, int y)
{
	int		index;
	uint8_t	*pixels;

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
		return (0);
	index = (y * texture->width + x) * 4;
	pixels = texture->pixels;

	uint8_t r = pixels[index];
	uint8_t g = pixels[index + 1];
	uint8_t b = pixels[index + 2];
	uint8_t a = pixels[index + 3];

	// Combine into RGBA format
	return (r << 24) | (g << 16) | (b << 8) | a;
}

void ft_draw_sky_floor(t_cub *cube)
{
	int	i;
	int	j;
	int	sky_end_y;
	int	floor_st_y;

	sky_end_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;
	floor_st_y = HEIGHT / 2 - cube->player->player_z - cube->player->jump_var;
	if (sky_end_y < 0)
		sky_end_y = 0;
	if (floor_st_y >= HEIGHT)
		floor_st_y = HEIGHT - 1;
	i = 0;
	while (i < WIDTH)
	{
		j = 0;
		while (j < HEIGHT)
		{
			if (j < sky_end_y)
				mlx_put_pixel(cube->image, i, j, c_rgba(cube->data->sky.r, cube->data->sky.g, cube->data->sky.b, 255));
			else if (j >= floor_st_y)
				mlx_put_pixel(cube->image, i, j, c_rgba(cube->data->floor.r, cube->data->floor.g, cube->data->floor.b, 255));
			++j;
		}
		++i;
	}
}

uint32_t	ft_shaded_color(uint32_t color, double shade)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = ((color >> 24) & 0xFF) * shade;
	g = ((color >> 16) & 0xFF) * shade;
	b = ((color >> 8) & 0xFF) * shade;
	return ((r << 24) | (g << 16) | (b << 8) | (color & 0xFF));
}

t_doors	*ft_get_smallest_dist(t_doors *head)
{
	t_doors	*low;

	low = head;
	while (head)
	{
		if (head->next && head->next->distance < low->distance)
			low = head->next;
		head = head->next;
	}
	return (low);
}

void	ft_get_texture(t_cub *cube, t_vars vars, int textureNum, int i, int door)
{
	mlx_texture_t	*texture;
	double			texturePosX;

	texture = NULL;
	if (vars.door)
		texture = cube->doors[0];
	else
		texture = cube->texture[textureNum];

	texturePosX = vars.wasHitVert ? fmod(vars.wallHitY, tile_size) / tile_size : fmod(vars.wallHitX, tile_size) / tile_size;
	texturePosX = 1.0 - texturePosX;

	int textureX = (int)(texturePosX * texture->width);
	double texturePos = vars.textureOffsetY * vars.textureStep;
	double shade = fmax(0.2, 1.0 - (vars.distance / 500.0));

	for (int y = vars.wallTopPixel; y < vars.wallBottomPixel && y < HEIGHT; y++)
	{
		int textureY = (int)(texturePos * texture->height) % texture->height;
		texturePos += vars.textureStep;
		uint32_t color = get_pixel_color(texture, textureX, textureY);
		if (!(door && ((ft_shaded_color(color, shade) & 0xFFFFFF00) == 0)) && y >= 0)
			mlx_put_pixel(cube->image, i, y, ft_shaded_color(color, shade));
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

void draw_gun_right_click(t_cub *cube)
{
	static int previous_gun_index = -1;

	int gun_index = cube->cur_g_right_clikc;

	if (previous_gun_index != -1 && cube->gun_r_img[previous_gun_index])
	{
		mlx_delete_image(cube->mlx, cube->gun_r_img[previous_gun_index]);
		cube->gun_r_img[previous_gun_index] = NULL;
	}

	if (!cube->gun_r_img[gun_index])
	{
		cube->gun_r_img[gun_index] = mlx_texture_to_image(cube->mlx, cube->gun_r[gun_index]);
		if (!cube->gun_r_img[gun_index])
			ft_error();
	}
	if (mlx_image_to_window(cube->mlx, cube->gun_r_img[gun_index], WIDTH / 2 - cube->gun_r[gun_index]->width / 2, HEIGHT - cube->gun_r[gun_index]->height) < 0)
		ft_error();

	previous_gun_index = gun_index;
}

void update_y_press(t_cub *cube)
{
	static int prev_gun_index = -1;

	if (cube->y_press)
	{
		if (prev_gun_index != -1 && cube->gun_img[prev_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);
			cube->gun_img[prev_gun_index] = NULL;
		}

		if (!cube->gun_img[cube->current_gun_index])
		{
			cube->gun_img[cube->current_gun_index] = mlx_texture_to_image(cube->mlx, cube->gun[cube->current_gun_index]);
			if (!cube->gun_img[cube->current_gun_index])
				ft_error();
		}
		if (mlx_image_to_window(cube->mlx, cube->gun_img[cube->current_gun_index],
								WIDTH / 2 - cube->gun[cube->current_gun_index]->width / 2,
								HEIGHT - cube->gun[cube->current_gun_index]->height) < 0)
			ft_error();

		prev_gun_index = cube->current_gun_index;
		if (cube->current_gun_index == Y_CLICK - 1)
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);	
			cube->y_press = 0;
		}
	}
}

void update_t_press(t_cub *cube)
{
	static int prev_gun_index;

	prev_gun_index = -1;
	if (cube->t_press)
	{
		if (prev_gun_index != -1 && cube->gun_img[prev_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);
			cube->gun_img[prev_gun_index] = NULL;
		}
		if (!cube->gun_img[0])
		{
			cube->gun_img[0] = mlx_texture_to_image(cube->mlx, cube->gun[0]);
			if (!cube->gun_img[0])
				ft_error();
		}
		if (mlx_image_to_window(cube->mlx, cube->gun_img[0],
								WIDTH / 2 - cube->gun[0]->width / 2,
								HEIGHT - cube->gun[0]->height) < 0)
			ft_error();
		prev_gun_index = 0;
	}
}

void *draw_lines_3D(void *tmp)
{
	t_cub *cube = (t_cub *)tmp;
	double distanceProjPlane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
	double angleStep = FOV_ANGLE / WIDTH;
	cube->angle_0 = cube->player->rotat_angle - FOV_ANGLE / 2.0;

	for (int i = 0; i < WIDTH / 2; i++)
	{
		t_vars vars = draw_line(cube, cube->angle_0, 0);
		double fx = cube->player->player_x;
		double fy = cube->player->player_y;
		if (vars.isRayFacingLeft)
			fx--;
		if (vars.isRayFacingUp)
			fy--;
		int x = floor(fx / tile_size);
		int y = floor(fy / tile_size);

		// if next qube is D open / up down right left
		if (cube->player->open == -1)
		{
			if (y != 0 && cube->data->map[y - 1][x] == 'D')
			{
				cube->data->map[y - 1][x] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
			if (y != cube->data->map_row && cube->data->map[y + 1][x] == 'D')
			{
				cube->data->map[y + 1][x] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
			if (x != 0 && cube->data->map[y][x - 1] == 'D')
			{
				cube->data->map[y][x - 1] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
			if (x != cube->data->map_cols && cube->data->map[y][x + 1] == 'D')
			{
				cube->data->map[y][x + 1] = 'T';
				vars = draw_line(cube, cube->angle_0, 0);
			}
		}
		// end if next qube is D open / up down right left 

		// if im awaay of the door close it / up down right left
		if (y != 1 && cube->data->map[y - 2][x] == 'T')
		{
			cube->data->map[y - 2][x] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 2][x] == 'T')
		{
			cube->data->map[y + 2][x] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != 1 && cube->data->map[y][x - 2] == 'T')
		{
			cube->data->map[y][x - 2] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y][x + 2] == 'T')
		{
			cube->data->map[y][x + 2] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		// end if im awaay of the door close it / up down right left

		// if im awaay of the door close it / up-right down-right up-left down-left
		if (y != 1 && cube->data->map[y - 1][x - 1] == 'T')
		{
			cube->data->map[y - 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 1][x + 1] == 'T')
		{
			cube->data->map[y + 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != 1 && cube->data->map[y + 1][x - 1] == 'T')
		{
			cube->data->map[y + 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y - 1][x + 1] == 'T')
		{
			cube->data->map[y - 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_0, 0);
		}
		// end if im awaay of the door close it / up-right down-right up-left down-left
		
		double wallDistance = vars.distance * cos(cube->angle_0 - cube->player->rotat_angle);
		double wallStripHeight = (tile_size / wallDistance) * distanceProjPlane;

		vars.wallTopPixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var;
		vars.wallBottomPixel = fmin((HEIGHT / 2.0) + (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var, HEIGHT);

		vars.textureStep = 1.0 / wallStripHeight;
		vars.textureOffsetY = 0;

		int textureNum = vars.wasHitVert ?
			(vars.isRayFacingLeft ? 2 : 3) :
			(vars.isRayFacingUp ? 0 : 1);
		ft_get_texture(cube, vars, textureNum, i, (cube->doortype / 2));
		cube->angle_0 += angleStep;
	}
	return NULL;
}

void *draw_lines_3D_1(void *tmp)
{
	t_cub *cube = (t_cub *)tmp;
	double distanceProjPlane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
	double angleStep = FOV_ANGLE / WIDTH;
	cube->angle_1 = cube->player->rotat_angle;

	for (int i = (WIDTH / 2); i < WIDTH; i++)
	{
		t_vars vars = draw_line(cube, cube->angle_1, 0);
		double fx = cube->player->player_x;
		double fy = cube->player->player_y;
		if (vars.isRayFacingLeft)
			fx--;
		if (vars.isRayFacingUp)
			fy--;
		int x = floor(fx / tile_size);
		int y = floor(fy / tile_size);
	
		// if next qube is D open / up down right left
		if (cube->player->open == -1)
		{
			if (y != 0 && cube->data->map[y - 1][x] == 'D')
			{
				cube->data->map[y - 1][x] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
			if (y != cube->data->map_row && cube->data->map[y + 1][x] == 'D')
			{
				cube->data->map[y + 1][x] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
			if (x != 0 && cube->data->map[y][x - 1] == 'D')
			{
				cube->data->map[y][x - 1] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
			if (x != cube->data->map_cols && cube->data->map[y][x + 1] == 'D')
			{
				cube->data->map[y][x + 1] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
		}
		// end if next qube is D open / up down right left 

		// if im awaay of the door close it / up down right left
		if (y != 1 && cube->data->map[y - 2][x] == 'T')
		{
			cube->data->map[y - 2][x] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 2][x] == 'T')
		{
			cube->data->map[y + 2][x] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != 1 && cube->data->map[y][x - 2] == 'T')
		{
			cube->data->map[y][x - 2] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y][x + 2] == 'T')
		{
			cube->data->map[y][x + 2] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		// end if im awaay of the door close it / up down right left

		// if im awaay of the door close it / up-right down-right up-left down-left
		if (y != 1 && cube->data->map[y - 1][x - 1] == 'T')
		{
			cube->data->map[y - 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 1][x + 1] == 'T')
		{
			cube->data->map[y + 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != 1 && cube->data->map[y + 1][x - 1] == 'T')
		{
			cube->data->map[y + 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y - 1][x + 1] == 'T')
		{
			cube->data->map[y - 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		// end if im awaay of the door close it / up-right down-right up-left down-left
		
		double wallDistance = vars.distance * cos(cube->angle_1 - cube->player->rotat_angle);
		double wallStripHeight = (tile_size / wallDistance) * distanceProjPlane;

		vars.wallTopPixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var;
		vars.wallBottomPixel = fmin((HEIGHT / 2.0) + (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var, HEIGHT);

		vars.textureStep = 1.0 / wallStripHeight;
		vars.textureOffsetY = 0;

		int textureNum = vars.wasHitVert ?
			(vars.isRayFacingLeft ? 2 : 3) :
			(vars.isRayFacingUp ? 0 : 1);
		ft_get_texture(cube, vars, textureNum, i, (cube->doortype / 2));
		cube->angle_1 += angleStep;
	}
	return NULL;
}

void my_mousehook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	(void)mods;
	t_cub *cube = (t_cub *)param;

	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
		cube->right_press = 1;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_RELEASE)
		cube->right_press = 0;
}
// hooks

void	ft_realse(t_cub *cube, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_O)
		cube->player->open *= -1;
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
	if (keydata.key == MLX_KEY_LEFT_CONTROL || keydata.key == MLX_KEY_LEFT_SHIFT)
		cube->player->jump = 0;
	if (keydata.key == MLX_KEY_RIGHT_SHIFT)
		cube->player->shift *= -1;
	if (keydata.key == MLX_KEY_M)
		cube->player->minimap *= -1;
	if (keydata.key == MLX_KEY_TAB)
		cube->player->tab = 0;
}

void	ft_press(t_cub *cube, mlx_key_data_t keydata)
{
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
	if (keydata.key == MLX_KEY_TAB)
		cube->player->right_left = 1;
	if (keydata.key == MLX_KEY_RIGHT_SHIFT)
		cube->player->mouse = 1;
	if (keydata.key == MLX_KEY_LEFT_SHIFT)
		cube->player->jump = 2;
}

void	ft_press_1(t_cub *cube, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_Y)
	{
		if (cube->current_gun_index == Y_CLICK)
			cube->current_gun_index = 0;
		cube->t_press = 0;
		cube->y_press = 1;
	}
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		ft_free_data(cube);
		mlx_close_window(cube->mlx);
		mlx_delete_image(cube->mlx, cube->image);
	}
	if (keydata.key == MLX_KEY_TAB)
		cube->player->tab = 1;
	if (keydata.key == MLX_KEY_LEFT_CONTROL)
		cube->player->jump = -1;
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_cub * cube = param;
	if (keydata.action == MLX_PRESS)
	{
		ft_press(cube, keydata);
		ft_press_1(cube, keydata);
	}
	if (keydata.action == MLX_RELEASE)
	{
		ft_realse(cube, keydata);
		if (keydata.key == MLX_KEY_Y)
		{
			cube->t_press = 0;
			if (cube->gun_img[cube->current_gun_index])
			{
				mlx_delete_image(cube->mlx, cube->gun_img[cube->current_gun_index]);
				cube->gun_img[cube->current_gun_index] = NULL;
			}
			if (cube->gun_img[0])
			{
				mlx_delete_image(cube->mlx, cube->gun_img[0]);
				cube->gun_img[0] = NULL;
			}
		}
	}
}

void handle_mouse(t_cub *cube)
{
	mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
	int32_t prev_xpos = WIDTH / 2;
	int32_t xpos;
	int32_t ypos;
	double sensitivity = 0.001;

	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

	int32_t delta_x = xpos - prev_xpos;

	cube->player->rotat_angle += delta_x * sensitivity;

	mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);

	int32_t prev_ypos = HEIGHT / 2;
	int32_t delta_y = ypos - prev_ypos;

	cube->player->player_z = cube->player->player_z + delta_y;
}

void update_run_on_right_click(t_cub *cube)
{
	static double last_gun_change_time = 0;
	double current_time = mlx_get_time();

	if (cube->right_press)
	{
		if ((current_time - last_gun_change_time) > 0.15)
		{
			if (cube->gun_r_img[cube->cur_g_right_clikc])
			{
				mlx_delete_image(cube->mlx, cube->gun_r_img[cube->cur_g_right_clikc]);
				cube->gun_r_img[cube->cur_g_right_clikc] = NULL;
			}

			cube->cur_g_right_clikc++;
			if (cube->cur_g_right_clikc > 1)
				cube->cur_g_right_clikc = 0;
			last_gun_change_time = current_time;
		}
	}
	else
		cube->cur_g_right_clikc = 0;
}

int calculateDistance(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

// void	inti_jump(t_cub *cube)
// {

// }

void update_player(t_cub *cube)
{
	static double last_gun_change_time = 0;
	double current_time = mlx_get_time();

	if (cube->y_press && (current_time - last_gun_change_time) > 0.15)
	{
		if (cube->gun_img[cube->current_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[cube->current_gun_index]);
			cube->gun_img[cube->current_gun_index] = NULL;
		}
		cube->current_gun_index++;
		if (cube->current_gun_index >= Y_CLICK)
			cube->current_gun_index = 0;
		last_gun_change_time = current_time;
	}
	// inti_jump(cube);
	// printf("%f\n", cube->player->jump);
	if (cube->player->jump == 1)
		cube->player->jump_var = -100;
	else if (cube->player->jump == -1)
	{
		cube->player->move_speed = 3;
		cube->player->jump_var = 100;
	}
	else if (cube->player->jump == 0)
	{
		cube->player->move_speed = 3;
		cube->player->jump_var = 0;
	}
	else if (cube->player->jump == 2)
		cube->player->move_speed = 2;

	int move_speed = cube->player->move_speed;
	cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
	cube->player->rotat_angle += (double)cube->player->turn_direction * cube->player->rotation_speed;

	while (move_speed--)
	{
		// Calculate movement step
		double movestep = (double)cube->player->walk_direction * move_speed;
		double new_player_x;
		double new_player_y;
		new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
		new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

		if (cube->player->strafe_direction != 0)
		{
			new_player_x += (double)cube->player->strafe_direction * (move_speed / 1.5) * cos(cube->player->rotat_angle + M_PI / 2);
			new_player_y += (double)cube->player->strafe_direction * (move_speed / 1.5) * sin(cube->player->rotat_angle + M_PI / 2);
		}

		// Check for wall collision before updating player position
		if (is_it_a_wall(new_player_x, new_player_y, cube))
		{
			// No collision, update both x and y
			cube->player->player_x = new_player_x;
			cube->player->player_y = new_player_y;
		}
		else
		{
			// Check for sliding along the wall
			if (is_it_a_wall(cube->player->player_x, new_player_y, cube))
				cube->player->player_y = new_player_y;
			if (is_it_a_wall(new_player_x, cube->player->player_y, cube))
				cube->player->player_x = new_player_x;
		}
	}
	// check if player is near a door
	t_doors *tmp = cube->doors_locations;
	int x = floor(cube->player->player_x / tile_size);
	int y = floor(cube->player->player_y / tile_size);
	while (tmp)
	{
		tmp->distance = calculateDistance(x, y, tmp->x, tmp->y);
		tmp = tmp->next;
	}
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
			if (i != 0 && y != 0 && y != 20 && i != 400)
				mlx_put_pixel(cube->image, i + 10, y + 10, c_rgba(0, 0, 255, 255));
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
			if (i == 0 || y == 0 || y == 20 || i == 400)
				mlx_put_pixel(cube->image, i + 10, y + 10, c_rgba(255, 0, 0, 255));
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
			if (i == 135)
				mlx_put_pixel(cube->image, i + WIDTH / 1.2, y + HEIGHT / 1.2, c_rgba(0, 0, 255, 255));
			else
				mlx_put_pixel(cube->image, i + WIDTH / 1.2, y + HEIGHT / 1.2, c_rgba(0, 0, 0, 255));
			y++;
		}
		i++;
	}
}
// end shots

#include <time.h>

#define TARGET_FPS 60
#define FRAME_TARGET_TIME (1000 / TARGET_FPS)

void loop_fun(void* param)
{
	t_cub* cube = (t_cub*)param;
	static clock_t previous_frame_time = 0;
	clock_t current_time = clock();
	double delta_time = (current_time - previous_frame_time) / (double)CLOCKS_PER_SEC;

	if (delta_time >= 1.0 / TARGET_FPS)
	{
		update_player(cube);
		ft_draw_sky_floor(cube);
		cube->threads[0].id = 0;
		cube->threads[1].id = 1;
		pthread_create(&cube->threads[0].thread, NULL, &draw_lines_3D, (void *)cube);
		pthread_create(&cube->threads[1].thread, NULL, &draw_lines_3D_1, (void *)cube);
		pthread_join(cube->threads[0].thread, NULL);
		pthread_join(cube->threads[1].thread, NULL);

		draw_per(cube);
		update_run_on_right_click(cube);
		update_y_press(cube);
		draw_gun_right_click(cube);
		previous_frame_time = current_time;
	}
	clock_t frame_time = clock() - current_time;
	if (frame_time < FRAME_TARGET_TIME)
	{
		struct timespec rem, req = {0, (FRAME_TARGET_TIME - frame_time) * 1000000};
		nanosleep(&req, &rem);
	}
	// Mouse handling
	int32_t xpos, ypos;
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && (cube->player->start == 0 || cube->player->start == 1))
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start += 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
// // end hooks
