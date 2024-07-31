#include "../recasting.h"
#include "../parsing.h"

void ffqwe()
{
	system("leaks app");
}

void my_mousehook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	(void)mods;
	(void)param;
	(void)action;
	(void)button;
    // t_cub *cube = (t_cub *)param;

    // if (button == MLX_MOUSE_BUTTON_MIDDLE && action == MLX_PRESS && cube->player->middle == 0)
	// 	cube->player->middle = 1;
    // if (button == MLX_MOUSE_BUTTON_MIDDLE && action == MLX_RELEASE)
	// 	cube->player->middle = 0;
    // if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	// {
	// 	int i = 0;
	// 	while (i < 2)
	// 	{
	// 		if (mlx_image_to_window(cube->mlx, cube->gun_img[i], WIDTH/ 2, HEIGHT - cube->gun[i]->height) < 0)
	// 			return ;
	// 		// mlx_delete_image(cube->mlx, cube->gun_img[i]);
	// 		i++;
	// 	}
	// 		if (mlx_image_to_window(cube->mlx, cube->gun_img[i], WIDTH/ 2, HEIGHT - cube->gun[i]->height) < 0)
	// 			return ;
	// }
}

int main()
{
	// atexit(ffqwe);
	int fd = open("../textures/Maps/map_1.cub", O_RDONLY);
	char *tmp;

	if (fd == -1 || fd == -1)
		return (1);
	t_init *init;
	t_data *data = NULL;

	data = (t_data *)ft_calloc(sizeof(t_data));
    init = (t_init *)ft_calloc(sizeof(t_init));
	if (!init)
		return (1);
	int i = 0;
	while (1)
	{
		tmp = get_next_line(fd);
		if (!tmp)
			break ;
		free(tmp);
		i++;
	}
	init->file_lines = i;
	close(fd);
	fd = open("../textures/Maps/map_1.cub", O_RDONLY);
	init->file = (char **)ft_calloc((i + 1) * sizeof(t_init *));
	i = 0;
	while (1)
	{
		init->file[i] = get_next_line(fd);
		if (!init->file[i])
			break ;
		i++;
	}
	close(fd);
	if (ft_get_data_init(init, data) == -1)
		return (1);
	if (ft_get_data(init, data) == -1)
		return (ft_check_map_print(data), 1);
    t_cub cube;
	cube.data = data;
	cube.data->height = cube.data->map_row * tile_size;
	cube.data->width = cube.data->map_cols * tile_size;
    ft_fractol_init(&cube);

	mlx_key_hook(cube.mlx, &my_keyhook, &cube);
	mlx_mouse_hook(cube.mlx, &my_mousehook, &cube);
    mlx_loop_hook(cube.mlx, loop_fun, &cube);
	mlx_loop(cube.mlx);
	// mlx_terminate(cube.mlx);

	// free_double_arr(init->file);
}
