#include "../../../recasting.h"

void	ft_error(void)
{
	fprintf(stderr, "%s\n", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	ft_free_data(t_cub *cube)
{
	free_double_arr(cube->data->map);
	free(cube->data->ea);
	free(cube->data->so);
	free(cube->data->no);
	free(cube->data->we);
	free(cube->player);
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
				player->player_x = (x * tile_size) + (tile_size / 2);
				player->player_y = (y * tile_size) + (tile_size / 2);
			}
		}
	}
	cube->doors_locations = head;
}
