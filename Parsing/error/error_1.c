#include "../parsing.h"

void	ft_check_color_print(t_data *data)
{
	ft_putstr_fd("Error: Check The Colors Provided\n", 2);
	// free_double_arr(data->coordinats);
	free_double_arr(data->map);
	//
	exit (1);
}

void	ft_print_cords_err(t_init *init)
{
	ft_putstr_fd("Error: Please Check The Coordinates\n", 2);
	free_double_arr(init->file);
	free_double_arr(init->map);
	exit (1);
}

void    ft_check_file(t_init *init)
{
	int i;

	i = 0;
	if (ft_count_arrgs_in_file(init, "NO") != 1
		|| ft_count_arrgs_in_file(init, "SO") != 1
		|| ft_count_arrgs_in_file(init, "WE") != 1
		|| ft_count_arrgs_in_file(init, "EA") != 1
		|| ft_count_arrgs_in_file(init, "F") != 1
		|| ft_count_arrgs_in_file(init, "C") != 1
		)
	{
		ft_putstr_fd("Error: Coordinates Missing\n", 2);
		free_double_arr(init->file);
		exit (1);
	}
}
