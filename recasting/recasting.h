#ifndef RECASTING_H
#define RECASTING_H

#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "mlx.h"
#include "../Parsing/parsing.h"

#define tile_size 50
// #define map_row 14
// #define map_cols 33

#define FOV_ANGLE M_PI / 3
#define WALL_STRIP_WIDTH 4
#define NUM_RAYS 400
#define line_leng 200

#define WIDTH map_cols * tile_size
#define HEIGHT map_row * tile_size

#define BLACK       0x000000
#define WHITE       0xFFFFFF
#define RED         0xFF0000
#define GREEN       0x00FF00
#define BLUE        0x0000FF

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
} t_img;

typedef struct s_player
{
	double	player_x;
	double	player_y;
	int	radius;
	int turn_direction;
	int walk_direction;
	double rotat_angle;
	double rotation_speed;
	double move_speed;
}	t_player;

typedef struct s_vars
{
	int isRayFacingDown;
	int isRayFacingUp;
	int isRayFacingRight;
	int isRayFacingLeft;
	int foundHorzWallHit;
	int foundVertWallHit;

	double horzWallHitX;
	double horzWallHitY;
	
	double y_intercept;
	double x_intercept;
	
	double ystep;
	double xstep;
	
	double nextHorzTouchX;
	double nextHorzTouchY;
	
	double vertWallHitX;
	double vertWallHitY;
	
	double nextVertTouchX;
	double nextVertTouchY;
	
	double horzHitDistance;
	double vertHitDistance;
	
	double wallHitX;
	double wallHitY;
	
	double angle;
}	t_vars;

typedef struct s_cub
{
	char **map;
	void	*mlx_con;
	void	*mlx_win;
	t_img	img;
	t_player *player;
	char	**colors;
	int		is;
	t_data	*data;
} t_cub;

void    ft_fractol_init(t_cub *cube);

// hooks
int loop_fun(t_cub * cube);
int handle_input_key_down(int keycode, t_cub * data);
int	handle_close_button(t_cub *data);
int handle_input_key_up(int keycode, t_cub * data);
// end hooks

#endif
