#include "../recasting.h"

void	ft_draw_hero(t_cub *cube, t_vars *vars)
{
	int	a;

	a = 0;
	vars->y_intercept = floor(cube->player->player_y / tile_size) * tile_size;
	if (vars->isRayFacingDown)
		vars->y_intercept += tile_size;
	vars->x_intercept = cube->player->player_x
		+ (vars->y_intercept - cube->player->player_y) / tan(vars->angle);
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
	if (vars->isRayFacingUp)
		a = 1;
	while (vars->nextHorzTouchX > 0
		&& vars->nextHorzTouchX < cube->data->map_cols * tile_size
		&& vars->nextHorzTouchY > 0
		&& vars->nextHorzTouchY < cube->data->map_row * tile_size)
	{
		if (has_wall(cube, vars->nextHorzTouchX, vars->nextHorzTouchY, a))
		{
			vars->foundHorzWallHit = 1;
			vars->horzWallHitX = vars->nextHorzTouchX;
			vars->horzWallHitY = vars->nextHorzTouchY;
			break ;
		}
		else
		{
			vars->nextHorzTouchX += vars->xstep;
			vars->nextHorzTouchY += vars->ystep;
		}
	}
}

void	ft_draw_ver(t_cub *cube, t_vars *vars)
{
	int	a;

	a = 0;
	vars->foundVertWallHit = 0;
	vars->vertWallHitX = 0;
	vars->vertWallHitY = 0;
	vars->x_intercept = floor(cube->player->player_x / tile_size) * tile_size;
	if (vars->isRayFacingRight)
		vars->x_intercept += tile_size;
	vars->y_intercept = cube->player->player_y
		+ (vars->x_intercept - cube->player->player_x) * tan(vars->angle);
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
	if (vars->isRayFacingLeft)
		a = 2;
	while (vars->nextVertTouchX > 0
		&& vars->nextVertTouchX < cube->data->map_cols * tile_size
		&& vars->nextVertTouchY > 0
		&& vars->nextVertTouchY < cube->data->map_row * tile_size)
	{
		if (has_wall(cube, vars->nextVertTouchX, vars->nextVertTouchY, a))
		{
			vars->foundVertWallHit = 1;
			vars->vertWallHitX = vars->nextVertTouchX;
			vars->vertWallHitY = vars->nextVertTouchY;
			break ;
		}
		else
		{
			vars->nextVertTouchX += vars->xstep;
			vars->nextVertTouchY += vars->ystep;
		}
	}
}

double	distanceBetweenPoints(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

t_vars	draw_line(t_cub *cube, double angle, int is)
{
	t_vars	vars;
	int		x;
	int		y;

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
		vars.horzHitDistance = distanceBetweenPoints(cube->player->player_x,
				cube->player->player_y, vars.horzWallHitX, vars.horzWallHitY);
	if (vars.foundVertWallHit)
		vars.vertHitDistance = distanceBetweenPoints(cube->player->player_x,
				cube->player->player_y, vars.vertWallHitX, vars.vertWallHitY);
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
	}
	else
	{
		vars.wallHitX = vars.horzWallHitX;
		vars.wallHitY = vars.horzWallHitY;
		vars.distance = vars.horzHitDistance;
		vars.wasHitVert = 0;
	}
	x = floor(vars.wallHitX / tile_size);
	y = floor(vars.wallHitY / tile_size);
	vars.door = 0;
	if (cube->data->map[y][x] == 'D')
		vars.door = 1;
	if (is == 1)
		DDA(cube, cube->player->player_x, cube->player->player_y,
			vars.wallHitX, vars.wallHitY);
	return (vars);
}

void	draw_lines(t_cub *cube, int is)
{
	int		i;
	double	angle;

	cube->is = MAP_SCALE;
	i = 0;
	angle = cube->player->rotat_angle + (FOV_ANGLE / 2);
	while (i < NUM_RAYS)
	{
		draw_line(cube, angle, is);
		angle -= FOV_ANGLE / NUM_RAYS;
		i++;
	}
}

static void	ft_error(void)
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
	free(cube->data);
	free(cube->player);
}

void	malloc_error(void)
{
	printf("Error: in malloc\n");
	exit(EXIT_FAILURE);
}

double	deg2rad(double degrees)
{
	return (degrees * (M_PI / 180.0));
}

void	ft_get_player_pos(t_player *player , t_cub *cube)
{
	int	x;
	int	y;

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

t_player	*init_player(t_cub *cube)
{
	t_player	*player;

	player = (t_player *)ft_calloc(sizeof(t_player));
	if (!player)
		malloc_error();
	ft_get_player_pos(player, cube);
	player->radius = 10;
	player->move_speed = 4;
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
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

void ft_fractol_init(t_cub *cube)
{
	int	i;

	i = 0;
	cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();
	char *texture_files[] = {cube->data->no, cube->data->so, cube->data->we, cube->data->ea};
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
			mlx_put_pixel(cube->image, x, y, create_rgba(0, 0, 0, 150));
	}
}
// end all_black

// draw_player

int is_it_a_wall(double x, double y, t_cub *cube)
{
	double	left = x - ((double)cube->player->radius);
	double	up = y - ((double)cube->player->radius);
	double	right = x + ((double)cube->player->radius);
	double	down = y + ((double)cube->player->radius);

	if (left < 0 || right > cube->data->map_cols * tile_size
		|| up < 0 || down > cube->data->map_row * tile_size)
		return (0);

	int  t_left = floor(left / tile_size);
	int  t_up = floor(up / tile_size);
	int  t_right = floor(right / tile_size);
	int  t_down = floor(down / tile_size);

	if (cube->data->map[t_up][t_left] == '1'
		|| cube->data->map[t_down][t_right] == '1'
		|| cube->data->map[t_up][t_right] == '1'
		|| cube->data->map[t_down][t_left] == '1')
		return (0);
	return (1);
}


void DDA(t_cub *cube, double X0, double Y0, double X1, double Y1)
{
	double	dx = X1 - X0;
	double	dy = Y1 - Y0;
	double	steps = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
	double	Xinc = dx / steps;
	double	Yinc = dy / steps;
	double	X = X0;
	double	Y = Y0;
	int i;

	i = 0;
	while (i <= steps)
	{
		mlx_put_pixel(cube->image, X, Y, create_rgba(250, 100, 100, 255));
		X += Xinc;
		Y += Yinc;
		i++;
	}
}

int has_wall(t_cub *cube, double x1, double y1, int is)
{
	int	x;
	int	y;

	if (is == 1)
		y1--;
	else if (is == 2)
		x1--;
	x = floor(x1 / tile_size);
	y = floor(y1 / tile_size);
	if (cube->data->map[y][x] == '1' || cube->data->map[y][x] == 'D')
		return (1);
	return (0);
}

double normalizeAngle(double angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}

// hooks
void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_cub	*cube = param;

	(void)keydata;
	(void)param;
	if (keydata.action == MLX_PRESS)
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

		if (keydata.key == MLX_KEY_ESCAPE)
		{
			ft_free_data(cube);
			mlx_close_window(cube->mlx);
			mlx_delete_image(cube->mlx, cube->image);
		}
	}
	if (keydata.action == MLX_RELEASE)
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
	}
}

void handle_mouse(t_cub *cube)
{
	static int32_t prev_xpos = WIDTH / 2;
	int32_t xpos, ypos;
	double sensitivity = 0.001;

	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	if (xpos < 0 || xpos > WIDTH || ypos < 0 || ypos > HEIGHT)
		return ;
	int32_t delta_x = xpos - prev_xpos;
	cube->player->rotat_angle += delta_x * sensitivity;
	mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
	mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_HIDDEN);
}

void update_player(t_cub *cube)
{
	int move_speed = cube->player->move_speed;
	double	new_player_x;
	double	new_player_y;

	cube->player->rotat_angle = normalizeAngle(cube->player->rotat_angle);
	cube->player->rotat_angle += (double)cube->player->turn_direction * cube->player->rotation_speed;
	while (move_speed--)
	{
		double movestep = (double)cube->player->walk_direction * move_speed;
		new_player_x = cube->player->player_x + movestep * cos(cube->player->rotat_angle);
		new_player_y = cube->player->player_y + movestep * sin(cube->player->rotat_angle);

		if (cube->player->strafe_direction != 0)
		{
			new_player_x += (double)cube->player->strafe_direction * (move_speed / 1.5) * cos(cube->player->rotat_angle + M_PI / 2);
			new_player_y += (double)cube->player->strafe_direction * (move_speed / 1.5) * sin(cube->player->rotat_angle + M_PI / 2);
		}
		if (is_it_a_wall(new_player_x, new_player_y, cube))
		{
			cube->player->player_x = new_player_x;
			cube->player->player_y = new_player_y;
		}
		else
		{
			if (is_it_a_wall(cube->player->player_x, new_player_y, cube))
				cube->player->player_y = new_player_y;
			if (is_it_a_wall(new_player_x, cube->player->player_y, cube))
				cube->player->player_x = new_player_x;
		}
	}
}

void loop_fun(void* param)
{
	t_cub	*cube;
	int32_t	xpos;
	int32_t	ypos;

	cube = (t_cub *)param;
	update_player(cube);
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	draw_lines_3d(cube);
	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && cube->player->start == 0)
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start = 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
// // end hooks
