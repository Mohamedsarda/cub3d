/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recasting_6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:03:12 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:03:13 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../recasting.h"

void	my_mousehook(mouse_key_t bt, action_t ac, modifier_key_t md, void *pa)
{
	t_cub	*cube;

	(void)md;
	cube = (t_cub *)pa;
	if (bt == MLX_MOUSE_BUTTON_LEFT && ac == MLX_PRESS)
		cube->right_press = 1;
	if (bt == MLX_MOUSE_BUTTON_LEFT && ac == MLX_RELEASE)
		cube->right_press = 0;
}

void	ft_realse(t_cub *cube, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_O)
		cube->player->open *= -1;
	if (keydata.key == MLX_KEY_W)
		cube->player->walk_direction = 0;
	if (keydata.key == MLX_KEY_DOWN)
		cube->player->walk_direction = 0;
	if (keydata.key == MLX_KEY_S)
		cube->player->walk_direction = 0;
	if (keydata.key == MLX_KEY_UP)
		cube->player->walk_direction = 0;
	if (keydata.key == MLX_KEY_RIGHT || keydata.key == MLX_KEY_LEFT)
		cube->player->turn_direction = 0;
	if (keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_A)
		cube->player->strafe_direction = 0;
	if (keydata.key == MLX_KEY_LEFT_CONTROL
		|| keydata.key == MLX_KEY_LEFT_SHIFT)
		cube->player->jump = 0;
	if (keydata.key == MLX_KEY_RIGHT_SHIFT)
		cube->player->shift *= -1;
	if (keydata.key == MLX_KEY_M)
		cube->player->minimap *= -1;
	if (keydata.key == MLX_KEY_TAB)
		cube->player->tab = 0;
}

void	ft_press(t_cub *cube, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_RIGHT)
		cube->player->turn_direction = 1;
	if (keydata.key == MLX_KEY_LEFT)
		cube->player->turn_direction = -1;
	if (keydata.key == MLX_KEY_W)
		cube->player->walk_direction = 1;
	if (keydata.key == MLX_KEY_UP)
		cube->player->walk_direction = 1;
	if (keydata.key == MLX_KEY_S)
		cube->player->walk_direction = -1;
	if (keydata.key == MLX_KEY_DOWN)
		cube->player->walk_direction = -1;
	if (keydata.key == MLX_KEY_D)
		cube->player->strafe_direction = 1;
	if (keydata.key == MLX_KEY_A)
		cube->player->strafe_direction = -1;
	if (keydata.key == MLX_KEY_TAB)
		cube->player->right_left = 1;
	if (keydata.key == MLX_KEY_RIGHT_SHIFT)
		cube->player->mouse = 1;
	if (keydata.key == MLX_KEY_LEFT_SHIFT)
		cube->player->jump = 2;
}

void	ft_press_1(t_cub *cube, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_Y)
	{
		if (cube->current_gun_index == Y_CLICK)
			cube->current_gun_index = 0;
		cube->t_press = 0;
		cube->y_press = 1;
	}
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		ft_free_data(cube);
		mlx_close_window(cube->mlx);
		mlx_delete_image(cube->mlx, cube->image);
		exit(0);
	}
	if (keydata.key == MLX_KEY_TAB)
		cube->player->tab = 1;
	if (keydata.key == MLX_KEY_LEFT_CONTROL)
		cube->player->jump = -1;
}

void	ft_get_player_pos(t_player *player, t_cub *cube)
{
	int		x;
	int		y;

	y = -1;
	while (++y < cube->data->map_row)
	{
		x = -1;
		while (++x < cube->data->map_cols)
		{
			if (cube->data->map[y][x] == cube->data->p)
			{
				player->player_x = (x * TILE_SIZE) + (TILE_SIZE / 2);
				player->player_y = (y * TILE_SIZE) + (TILE_SIZE / 2);
			}
		}
	}
}
