#include "../../../recasting.h"

int	is_it_a_wall(double x, double y, t_cub *cube)
{
	t_het_wall	player;

	player.left = x - ((double)cube->player->radius);
	player.up = y - ((double)cube->player->radius);
	player.right = x + ((double)cube->player->radius);
	player.down = y + ((double)cube->player->radius);
	if (player.left < 0 || player.right > cube->data->map_cols * TILE_SIZE
		|| player.up < 0 || player.down > cube->data->map_row * TILE_SIZE)
		return (0);
	player.t_left = floor(player.left / TILE_SIZE);
	player.t_up = floor(player.up / TILE_SIZE);
	player.t_right = floor(player.right / TILE_SIZE);
	player.t_down = floor(player.down / TILE_SIZE);
	if (cube->data->map[player.t_up][player.t_left] == '1'
		|| cube->data->map[player.t_down][player.t_right] == '1'
		|| cube->data->map[player.t_up][player.t_right] == '1'
		|| cube->data->map[player.t_down][player.t_left] == '1')
		return (0);
	if ((cube->data->map[player.t_up][player.t_left] == 'D'
		|| cube->data->map[player.t_down][player.t_right] == 'D'
		|| cube->data->map[player.t_up][player.t_right] == 'D'
		|| cube->data->map[player.t_down][player.t_left] == 'D')
		&& cube->player->open == 1)
		return (0);
	return (1);
}

void	draw_gun_right_click(t_cub *cube)
{
	static int	previous_gun_index;
	int			gun_index;

	gun_index = 10;
	if (cube->gun_img[previous_gun_index])
	{
		mlx_delete_image(cube->mlx, cube->gun_img[previous_gun_index]);
		cube->gun_img[previous_gun_index] = NULL;
	}
	if (!cube->gun_img[gun_index])
	{
		cube->gun_img[gun_index] = mlx_texture_to_image(cube->mlx,
				cube->gun[gun_index]);
		if (!cube->gun_img[gun_index])
			ft_error();
	}
	if (mlx_image_to_window(cube->mlx, cube->gun_img[gun_index],
			WIDTH / 2 - cube->gun[gun_index]->width / 2,
			HEIGHT - cube->gun[gun_index]->height) < 0)
		ft_error();
	previous_gun_index = gun_index;
}

int	texternum(t_vars vars)
{
	int		texture_num;

	texture_num = 0;
	if (vars.washitvert)
	{
		if (vars.israyfacingleft)
			texture_num = 2;
		else
			texture_num = 3;
	}
	else
	{
		if (vars.israyfacingup)
			texture_num = 0;
		else
			texture_num = 1;
	}
	return (texture_num);
}

void	init_vars_1(t_vars *vars, t_cub *cube, double dis_pr_plan, double angle)
{
	double	wall_distance;
	double	wall_strip_height;

	wall_distance = vars->distance
		* cos(angle - cube->player->rotat_angle);
	wall_strip_height = (TILE_SIZE / wall_distance) * dis_pr_plan;
	vars->walltoppixel = (HEIGHT / 2.0) - (wall_strip_height / 2.0)
		- cube->player->player_z - cube->player->jump_var;
	vars->wallbottompixel = fmin((HEIGHT / 2.0)
			+ (wall_strip_height / 2.0) - cube->player->player_z
			- cube->player->jump_var, HEIGHT);
	vars->texturestep = 1.0 / wall_strip_height;
	vars->textureoffsety = 0;
}

void	*draw_lines_3d(void *tmp)
{
	t_cub	*cube;
	double	distance_proj_plane;
	int		i;
	t_vars	vars;

	i = 0;
	cube = (t_cub *)tmp;
	distance_proj_plane = (WIDTH / 2.0) / tan((M_PI / 3) / 2);
	cube->angle_0 = cube->player->rotat_angle - (M_PI / 3) / 2.0;
	while (i < WIDTH / 2)
	{
		vars = draw_line(cube, cube->angle_0, 0);
		vars = open_door(vars, cube, cube->angle_0);
		init_vars_1(&vars, cube, distance_proj_plane, cube->angle_0);
		vars.door_var = (cube->doortype / 2);
		ft_get_texture_b(cube, vars, texternum(vars), i);
		cube->angle_0 += ((M_PI / 3) / WIDTH);
		i++;
	}
	return (NULL);
}
