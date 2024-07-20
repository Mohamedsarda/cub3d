#ifndef RECASTING_H
#define RECASTING_H

#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "mlx.h"

#define tile_size 50
#define map_row 11
#define map_cols 15

#define FOV_ANGLE 60 * ( M_PI / 180)
#define WALL_STRIP_WIDTH 4
#define NUM_RAYS 50
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


typedef struct s_cub
{
	int map[map_row][map_cols];
	void	*mlx_con;
	void	*mlx_win;
	t_img	img;
	t_player *player;
	char	**colors;
	int		is;
} t_cub;


#endif