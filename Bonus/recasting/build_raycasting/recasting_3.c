#include "../../../recasting.h"

char	**arr_guns(void)
{
	int		i;
	char	**guns;
	char	*tmp;
	char	*num;
	char	*str;

	i = -1;
	guns = (char **)ft_calloc(sizeof(char *) * (Y_CLICK + 1));
	if (!guns)
		return (NULL);
	while (++i < Y_CLICK)
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

void	ft_load_textures(t_cub *cube)
{
	int		i;
	char	*texture_files[4];

	i = 0;
	texture_files[0] = cube->data->no;
	texture_files[1] = cube->data->so;
	texture_files[2] = cube->data->we;
	texture_files[3] = cube->data->ea;
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
}

void	ft_load_guns(t_cub *cube)
{
	char	**guns;
	int		i;

	guns = arr_guns();
	if (!guns)
		return ;
	i = 0;
	while (i < Y_CLICK)
	{
		cube->gun[i] = mlx_load_png(guns[i]);
		if (!cube->gun[i])
			ft_error();
		cube->gun_img[i] = NULL;
		i++;
	}
	free_double_arr(guns);
}

void	ft_fractol_init(t_cub *cube)
{
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
	ft_load_guns(cube);
	ft_load_doors(cube, 0, "../Textures/Doors/tile000.png");
	ft_load_textures(cube);
	cube->player = init_player(cube);
}
