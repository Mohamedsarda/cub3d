/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DrawVer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:04:09 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:22:16 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../recasting.h"

void	close_window(void *param)
{
	t_cub	*cube;

	cube = (t_cub *)param;
	ft_free_data(cube);
	mlx_delete_image(cube->mlx, cube->image);
	mlx_close_window(cube->mlx);
}

void	ft_fractol_init(t_cub *cube)
{
	int		i;
	char	*texture_files[4];

	i = 0;
	texture_files[0] = cube->data->no;
	texture_files[1] = cube->data->so;
	texture_files[2] = cube->data->we;
	texture_files[3] = cube->data->ea;
	cube->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!cube->mlx)
		ft_error();
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image || (mlx_image_to_window(cube->mlx, cube->image, 0, 0) < 0))
		ft_error();
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

static void	ft_ver_calc(t_cub *cube, t_vars *vars)
{
	vars->foundvertwallhit = 0;
	vars->vertwallhitx = 0;
	vars->vertwallhity = 0;
	vars->x_intercept = floor(cube->player->player_x / TILE_SIZE) * TILE_SIZE;
	if (vars->israyfacingright)
		vars->x_intercept += TILE_SIZE;
	vars->y_intercept = cube->player->player_y
		+ (vars->x_intercept - cube->player->player_x) * tan(vars->angle);
	vars->xstep = TILE_SIZE;
	if (vars->israyfacingleft)
		vars->xstep *= -1;
	vars->ystep = TILE_SIZE * tan(vars->angle);
	if (vars->israyfacingup && vars->ystep > 0)
		vars->ystep *= -1;
	if (vars->israyfacingdown && vars->ystep < 0)
		vars->ystep *= -1;
	vars->nextverttouchx = vars->x_intercept;
	vars->nextverttouchy = vars->y_intercept;
}

void	ft_draw_ver(t_cub *cube, t_vars *vars)
{
	int	a;

	a = 0;
	ft_ver_calc(cube, vars);
	if (vars->israyfacingleft)
		a = 2;
	while (vars->nextverttouchx > 0
		&& vars->nextverttouchx < cube->data->map_cols * TILE_SIZE
		&& vars->nextverttouchy > 0
		&& vars->nextverttouchy < cube->data->map_row * TILE_SIZE)
	{
		if (has_wall(cube, vars->nextverttouchx, vars->nextverttouchy, a))
		{
			vars->foundvertwallhit = 1;
			vars->vertwallhitx = vars->nextverttouchx;
			vars->vertwallhity = vars->nextverttouchy;
			break ;
		}
		else
		{
			vars->nextverttouchx += vars->xstep;
			vars->nextverttouchy += vars->ystep;
		}
	}
}
