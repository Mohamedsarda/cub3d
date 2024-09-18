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

void	ft_get_player_pos(t_player *player, t_cub *cube)
{
	int		x;
	int		y;
	t_doors	*head;
	t_doors	*new;

	y = -1;
	head = NULL;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
		{
			if (cube->data->map[y][x] == 'D' || cube->data->map[y][x] == 'O')
			{
				new = ft_lstnew_doors(x, y);
				ft_lstaddback_doors(&head, new);
			}
			if (cube->data->map[y][x] == cube->data->p)
			{
				player->player_x = (x * TILE_SIZE) + (TILE_SIZE / 2);
				player->player_y = (y * TILE_SIZE) + (TILE_SIZE / 2);
			}
		}
	}
	cube->doors_locations = head;
}
