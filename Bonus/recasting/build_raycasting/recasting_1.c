#include "../../../recasting.h"

void	ft_error(void)
{
	fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	ft_free_data(t_cub *cube)
{
	int	i;

	i = -1;
	free_double_arr(cube->data->map);
	free(cube->data->ea);
	free(cube->data->so);
	free(cube->data->no);
	free(cube->data->we);
	free(cube->data);
	free(cube->player);
	mlx_delete_texture(cube->doors[0]);
	while (++i < 4)
	{
		mlx_delete_image(cube->mlx, cube->img[i]);
		mlx_delete_texture(cube->texture[i]);
	}
	i = -1;
	while (++i < Y_CLICK)
	{
		mlx_delete_image(cube->mlx, cube->img[i]);
		mlx_delete_texture(cube->gun[i]);
	}
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
