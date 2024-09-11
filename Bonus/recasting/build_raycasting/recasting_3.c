#include "../../../recasting.h"

char	**arr_guns(void)
{
	int		i;
	char	**guns;
	char	*tmp;
	char	*num;
	char	*str;

	i = -1;
	guns = (char **)ft_calloc(sizeof(char *) * (Y_CLICK + R_CLICK + 1));
	if (!guns)
		return (NULL);
	while (++i < Y_CLICK + R_CLICK)
	{
		tmp = ft_strdup("../Textures/png/guns_0/");
		num = ft_itoa(i);
		str = ft_strjoin(tmp, num);
		str = ft_strjoin(str, ".png");
		guns[i] = str;
		free(num);
	}
	guns[i] = NULL;
	return (guns);
}

// void	arr_guns_1(t_cub *cube, char **guns)
// {
// 	int		i;
// 	i = 0;
// 	while (i < Y_CLICK)
// 	{
// 		cube->gun[i] = mlx_load_png(guns[i]);
// 		if (!cube->gun[i])
// 			ft_error();
// 		if (i < 2)
// 		{
// 			cube->gun_img[i] = mlx_texture_to_image(cube->mlx, cube->gun[i]);
// 			if (!cube->gun_img[i])
// 				ft_error();
// 		}
// 		i++;
// 	}
// }

// void	arr_guns_2(t_cub *cube, char **guns)
// {
// 	int		i;
// 	int		k;
// 	i = 0;
// 	k = 0;
// 	while (guns[i])
// 	{
// 		cube->gun_r[k] = mlx_load_png(guns[i]);
// 		if (!cube->gun_r[k])
// 			ft_error();
// 		if (k < 2)
// 		{
// 			cube->gun_r_img[k] = mlx_texture_to_image(cube->mlx,
// 					cube->gun_r[k]);
// 			if (!cube->gun_r_img[k])
// 				ft_error();
// 		}
// 		k++;
// 		i++;
// 	}
// }

void	ft_fractol_init(t_cub *cube)
{
	char	**guns;
	int		i;
	int		k;

	i = 0;
	k = 0;
	cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();
	guns = arr_guns();
	if (!guns)
		return ;
	// arr_guns_1(cube, guns);
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
