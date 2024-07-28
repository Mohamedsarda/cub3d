#ifndef RECASTING_H
#define RECASTING_H

#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
// #include "mlx.h"
#include "../Parsing/parsing.h"
#include "../../MLX42/include/MLX42/MLX42.h"
#define tile_size 64
// #define map_row 14
// #define map_cols 33

#define FOV_ANGLE M_PI / 3
#define WALL_STRIP_WIDTH 1
#define NUM_RAYS 5000
#define line_leng 200

#define MAP_SCALE 1

#define WIDTH 1900
#define HEIGHT 1000

#define BLACK       0x000000
#define WHITE       0xFFFFFF
#define RED         0xFF0000FF
#define GREEN       0x98FB98
#define BLUE        0x0000FF
#define GRAY        0xC0C0C0

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
	int		*arr;
	int		width;
	int		height;
} t_img;


typedef struct s_player
{
	double	player_x;
	double	player_y;
	int	radius;
	int turn_direction;
	int strafe_direction;
	int walk_direction;
	double rotat_angle;
	double rotation_speed;
	double move_speed;
}	t_player;

typedef struct s_texture {
    void *img_ptr;
    char *data;
    int width;
    int height;
    int bpp;
    int line_len;
    int endian;
} t_texture;

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
	double distance;
	int		wasHitVert;
}	t_vars;

typedef struct s_cub
{
	char **map;
	// t_img	img;
	mlx_image_t* image;
	mlx_t* mlx;
	t_player *player;
	mlx_texture_t* texture[4];
	mlx_image_t* img[4];
	char	**colors;
	double		is;
	// t_texture texture[4];
	t_data	*data;
} t_cub;

void    ft_fractol_init(t_cub *cube);

// hooks
void loop_fun(void* param);
void my_keyhook(mlx_key_data_t keydata, void* param);
// end hooks

#endif
