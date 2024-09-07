#include "../recasting.h"

static void	ft_get_wallhit(t_vars *vars)
{
	vars->wallhitx = vars->vertwallhitx;
	vars->wallhity = vars->vertwallhity;
	vars->distance = vars->verthitdistance;
	vars->washitvert = 1;
	if (vars->verthitdistance < vars->horzhitdistance)
	{
		vars->wallhitx = vars->vertwallhitx;
		vars->wallhity = vars->vertwallhity;
		vars->distance = vars->verthitdistance;
		vars->washitvert = 1;
	}
	else
	{
		vars->wallhitx = vars->horzwallhitx;
		vars->wallhity = vars->horzwallhity;
		vars->distance = vars->horzhitdistance;
		vars->washitvert = 0;
	}
}

static void	ft_init_rays(t_vars *vars)
{
	vars->israyfacingdown = vars->angle > 0 && vars->angle < M_PI;
	vars->israyfacingup = !vars->israyfacingdown;
	vars->israyfacingright = vars->angle < 0.5 * M_PI
		|| vars->angle > 1.5 * M_PI;
	vars->israyfacingleft = !vars->israyfacingright;
	vars->foundhorzwallhit = 0;
	vars->horzwallhitx = 0;
	vars->horzwallhity = 0;
}

static void	ft_horz_or_ver(t_vars *vars, t_cub *cube)
{
	vars->horzhitdistance = __INT_MAX__;
	vars->verthitdistance = __INT_MAX__;
	if (vars->foundhorzwallhit)
		vars->horzhitdistance = distance_between_points(cube->player->player_x,
				cube->player->player_y, vars->horzwallhitx, vars->horzwallhity);
	if (vars->foundvertwallhit)
		vars->verthitdistance = distance_between_points(cube->player->player_x,
				cube->player->player_y, vars->vertwallhitx, vars->vertwallhity);
}

t_vars	draw_line(t_cub *cube, double angle, int is)
{
	t_vars	vars;
	int		x;
	int		y;

	(void)is;
	vars.angle = normalize_angle(angle);
	ft_init_rays(&vars);
	ft_draw_hero(cube, &vars);
	ft_draw_ver(cube, &vars);
	ft_horz_or_ver(&vars, cube);
	ft_get_wallhit(&vars);
	x = floor(vars.wallhitx / tile_size);
	y = floor(vars.wallhity / tile_size);
	vars.door = 0;
	if (cube->data->map[y][x] == 'D')
		vars.door = 1;
	return (vars);
}
<<<<<<< HEAD
=======

int	c_rgba(int r, int g, int b, int a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

void	ft_fractol_init(t_cub *cube)
{
	int	i;

	i = 0;
	char *texture_files[] = {cube->data->no, cube->data->so, cube->data->we, cube->data->ea};
	cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();
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

void	my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_cub	*cube;

	(void)keydata;
	(void)param;
	cube = param;
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

void	handle_mouse(t_cub *cube)
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

void	update_player(t_cub *cube)
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
>>>>>>> main
