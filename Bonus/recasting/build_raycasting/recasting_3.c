#include "../../../recasting.h"

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

void	ft_fractol_init(t_cub *cube)
{
	char	**guns;
	int		i;
	char	*tmp;
	char	*num;
	char	*str;
	int		k;

	i = 0;
	k = 0;
	cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();
	guns = (char **)ft_calloc(sizeof(char *) * (Y_CLICK + R_CLICK + 1));
	if (!guns)
		return ;
	while (i < Y_CLICK + R_CLICK)
	{
		tmp = ft_strdup("../Textures/png/guns_0/");
		num = ft_itoa(i);
		str = ft_strjoin(tmp, num);
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
