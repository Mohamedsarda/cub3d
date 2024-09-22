/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:02:28 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:04:07 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../recasting.h"

void	draw_lines(t_cub *cube, int is)
{
	int		i;
	double	angle;

	cube->is = MAP_SCALE;
	i = 0;
	angle = cube->player->rotat_angle + ((M_PI / 3) / 2);
	while (i < NUM_RAYS)
	{
		draw_line(cube, angle, is);
		angle -= (M_PI / 3) / NUM_RAYS;
		i++;
	}
}

void	loop_fun(void *param)
{
	t_cub	*cube;
	int32_t	xpos;
	int32_t	ypos;

	cube = (t_cub *)param;
	update_player(cube);
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);
	draw_lines_3d_m(cube);
	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && cube->player->start == 0)
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start = 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
