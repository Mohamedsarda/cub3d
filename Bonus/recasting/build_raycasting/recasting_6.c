#include "../../../recasting.h"

void	update_t_press(t_cub *cube)
{
	static int	prev_gun_index;

	prev_gun_index = -1;
	if (cube->t_press)
	{
		if (prev_gun_index != -1 && cube->gun_img[prev_gun_index])
		{
			mlx_delete_image(cube->mlx, cube->gun_img[prev_gun_index]);
			cube->gun_img[prev_gun_index] = NULL;
		}
		if (!cube->gun_img[0])
		{
			cube->gun_img[0] = mlx_texture_to_image(cube->mlx, cube->gun[0]);
			if (!cube->gun_img[0])
				ft_error();
		}
		if (mlx_image_to_window(cube->mlx, cube->gun_img[0],
				WIDTH / 2 - cube->gun[0]->width / 2,
				HEIGHT - cube->gun[0]->height) < 0)
			ft_error();
		prev_gun_index = 0;
	}
}

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
	}
	if (keydata.key == MLX_KEY_TAB)
		cube->player->tab = 1;
	if (keydata.key == MLX_KEY_LEFT_CONTROL)
		cube->player->jump = -1;
}
