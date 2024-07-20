#include "./recasting/recasting.h"
#include "./Parsing/parsing.h"



int main()
{
	// atexit(ff);
	int fd = open("./Parsing/Maps/map_1.cub", O_RDONLY);
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
	fd = open("./Parsing/Maps/map_1.cub", O_RDONLY);
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
    printf("%s\n", data->ea);
	if (ft_get_data(init, data) == -1)
		return (ft_check_map_print(data), 1);
   
    t_cub cube;
    ft_fractol_init(&cube, data->map);
    mlx_hook(cube.mlx_win, 17, 1L << 17, handle_close_button, &cube);
    mlx_hook(cube.mlx_win, 2, 0L, handle_input_key_down, &cube);
    mlx_hook(cube.mlx_win, 3, 0L, handle_input_key_up, &cube);
    mlx_loop_hook(cube.mlx_con, loop_fun, &cube);
	mlx_loop(cube.mlx_con);

	// free_double_arr(init->file);
}