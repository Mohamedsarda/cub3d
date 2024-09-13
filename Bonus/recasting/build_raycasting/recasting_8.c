#include "../../../recasting.h"

t_vars	open_door(t_vars vars, t_cub *cube, int x, int y)
{
	if (cube->player->open == -1)
	{
		if (y != 0 && cube->data->map[y - 1][x] == 'D')
		{
			cube->data->map[y - 1][x] = 'T';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (y != cube->data->map_row && cube->data->map[y + 1][x] == 'D')
		{
			cube->data->map[y + 1][x] = 'T';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != 0 && cube->data->map[y][x - 1] == 'D')
		{
			cube->data->map[y][x - 1] = 'T';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != cube->data->map_cols && cube->data->map[y][x + 1] == 'D')
		{
			cube->data->map[y][x + 1] = 'T';
			vars = draw_line(cube, cube->angle_1, 0);
		}
	}
	return (vars);
}

void	*draw_lines_3D_1(void *tmp)
{
	t_cub	*cube;
	double	distanceProjPlane;
	double	angleStep;
	double	fx;
	double	fy;
	double	wallDistance;
	double	wallStripHeight;
	int		x;
	int		y;
	int		textureNum;

	cube = (t_cub *)tmp;
	distanceProjPlane = (WIDTH / 2.0) / tan(FOV_ANGLE / 2);
	angleStep = FOV_ANGLE / WIDTH;
	cube->angle_1 = cube->player->rotat_angle;
	for (int i = (WIDTH / 2); i < WIDTH; i++)
	{
		t_vars vars = draw_line(cube, cube->angle_1, 0);
		fx = cube->player->player_x;
		fy = cube->player->player_y;
		if (vars.washitvert && vars.israyfacingleft)
			fx--;
		if (!vars.washitvert && vars.israyfacingup)
			fy--;
		x = floor(fx / tile_size);
		y = floor(fy / tile_size);
		// if next qube is D open / up down right left
		// vars = open_door(vars, cube, x, y);
		if (cube->player->open == -1)
		{
			if (y != 0 && cube->data->map[y - 1][x] == 'D')
			{
				cube->data->map[y - 1][x] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
			if (y != cube->data->map_row && cube->data->map[y + 1][x] == 'D')
			{
				cube->data->map[y + 1][x] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
			if (x != 0 && cube->data->map[y][x - 1] == 'D')
			{
				cube->data->map[y][x - 1] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
			if (x != cube->data->map_cols && cube->data->map[y][x + 1] == 'D')
			{
				cube->data->map[y][x + 1] = 'T';
				vars = draw_line(cube, cube->angle_1, 0);
			}
		}
		// end if next qube is D open / up down right left 

		// if im awaay of the door close it / up down right left
		if (y != 1 && cube->data->map[y - 2][x] == 'T')
		{
			cube->data->map[y - 2][x] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 2][x] == 'T')
		{
			cube->data->map[y + 2][x] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != 1 && cube->data->map[y][x - 2] == 'T')
		{
			cube->data->map[y][x - 2] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y][x + 2] == 'T')
		{
			cube->data->map[y][x + 2] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		// end if im awaay of the door close it / up down right left

		// if im awaay of the door close it / up-right down-right up-left down-left
		if (y != 1 && cube->data->map[y - 1][x - 1] == 'T')
		{
			cube->data->map[y - 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (y != cube->data->map_row - 2 && cube->data->map[y + 1][x + 1] == 'T')
		{
			cube->data->map[y + 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != 1 && cube->data->map[y + 1][x - 1] == 'T')
		{
			cube->data->map[y + 1][x - 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		if (x != cube->data->map_cols  - 2 && cube->data->map[y - 1][x + 1] == 'T')
		{
			cube->data->map[y - 1][x + 1] = 'D';
			vars = draw_line(cube, cube->angle_1, 0);
		}
		// end if im awaay of the door close it / up-right down-right up-left down-left
		
		wallDistance = vars.distance * cos(cube->angle_1 - cube->player->rotat_angle);
		wallStripHeight = (tile_size / wallDistance) * distanceProjPlane;

		vars.walltoppixel = (HEIGHT / 2.0) - (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var;
		vars.wallbottompixel = fmin((HEIGHT / 2.0) + (wallStripHeight / 2.0) - cube->player->player_z - cube->player->jump_var, HEIGHT);

		vars.texturestep = 1.0 / wallStripHeight;
		vars.textureoffsety = 0;

		textureNum = vars.washitvert ?
			(vars.israyfacingleft ? 2 : 3) :
			(vars.israyfacingup ? 0 : 1);
		ft_get_texture(cube, vars, textureNum, i, (cube->doortype / 2));
		cube->angle_1 += angleStep;
	}
	return NULL;
}