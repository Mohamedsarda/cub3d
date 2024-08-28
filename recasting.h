#ifndef RECASTING_H
# define RECASTING_H

# include <math.h>
# include <stdio.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
// # include "mlx.h"
# include "./parsing.h"
# include "../MLX42/include/MLX42/MLX42.h"
// # define map_row 14
// # define map_cols 33

//check map
// # define WIDTH 3000
// # define HEIGHT 2000

//check 3D
# define WIDTH 2000
# define HEIGHT 1000
# define tile_size 64
# define FOV_ANGLE M_PI / 3
# define WALL_STRIP_WIDTH 1
# define NUM_RAYS WIDTH
# define line_leng 200

# define MAP_SCALE 1

# define MINIMAP_SIZE 200
# define MINIMAP_X_OFFSET 20
# define MINIMAP_Y_OFFSET 20

# define BLACK       0x000000
# define WHITE       0xFFFFFF
# define RED         0xFF0000FF
# define GREEN       0x98FB98
# define BLUE        0x0000FF
# define GRAY        0xC0C0C0
# define R_CLICK 76
# define Y_CLICK 105

typedef struct s_doors
{
	int				x;
	int				y;
	int				distance;
	int				state;
	struct s_doors	*next;
}	t_doors;

typedef struct s_vars_3d
{
	double	distanceprojplane;
	double	angle;
	double	anglestep;
	double	walldistance;
	double	wallstripheight;
}	t_vars_3d;

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
}		t_img;

typedef struct s_pos
{
	int	x;
	int	y;
}		t_pos;

typedef struct s_player
{
	double	player_x;
	double	player_y;
	double	player_z;
	int		radius;
	int		turn_direction;
	int		strafe_direction;
	int		walk_direction;
	int		start;
	double	rotat_angle;
	double	rotation_speed;
	double	move_speed;
	double	is_moving_up;
	double	jump;
	double	jump_var;
	int		tab;
	int		middle;
	int		mouse;
	int		stop_mouse;
	int		right_left;
	int		shift;
	int		open;
	int		minimap;
	int		minimap_size;
	int		minimap_start_x;
	int		minimap_start_y;
	int		minimap_player_x;
	int		minimap_player_y;
}	t_player;

typedef struct s_texture
{
	void	*img_ptr;
	char	*data;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}		t_texture;

typedef struct s_vars
{
	int		isRayFacingDown;
	int		isRayFacingUp;
	int		isRayFacingRight;
	int		isRayFacingLeft;
	int		foundHorzWallHit;
	int		foundVertWallHit;

	double	y_intercept;
	double	x_intercept;

	double	ystep;
	double	xstep;

	double	nextHorzTouchX;
	double	nextHorzTouchY;

	double	horzWallHitX;
	double	horzWallHitY;

	double	vertWallHitX;
	double	vertWallHitY;

	double	nextVertTouchX;
	double	nextVertTouchY;

	double	horzHitDistance;
	double	vertHitDistance;

	double	wallHitX;
	double	wallHitY;

	double	distance;
	int		wasHitVert;

	double	angle;

	int		textureNum;
	double	textureStep;
	double	textureOffsetY;
	double	wallBottomPixel;
	double	wallTopPixel;
	int		door;
}	t_vars;

typedef struct s_minimap_vars
{
	double	world_x;
	double	world_y;
	int		map_x;
	int		map_y;
	int		color;
	int		screen_x;
	int		screen_y;
	int		dx;
	int		dy;
}	t_minimap_vars;

typedef struct s_sprite
{
	double		x;
	double		y;
	mlx_image_t	**images;
	int			num_images;
	int			current_image;
	double		animation_time;
}		t_sprite;

typedef struct s_threads
{
	int			id;
	pthread_t	thread;
}		t_threads;

typedef struct s_text_vars
{
	mlx_texture_t	*texture;
	double			texture_pos_x;
	double			texture_pos;
	double			shade;
	int				texture_x;
	int				texture_y;
}		t_text_vars;

typedef struct s_pixel_vars
{
	uint8_t	*pixels;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;
}		t_pixel_vars;

typedef struct s_cub
{
	char			**map;
	t_doors			*doors_locations;
	mlx_image_t		*image;
	mlx_t			*mlx;
	t_player		*player;
	mlx_texture_t	*texture[4];
	mlx_image_t		*img[4];
	mlx_texture_t	*gun[Y_CLICK];
	mlx_image_t		*gun_img[Y_CLICK];
	mlx_texture_t	*gun_r[R_CLICK];
	mlx_image_t		*gun_r_img[R_CLICK];
	mlx_texture_t	*doors[1];
	mlx_image_t		*door_img[1];
	char			**colors;
	double			is;
	int				y_press;
	int				t_press;
	int				right_press;
	int				current_gun_index;
	int				cur_g_right_clikc;
	int				doortype;
	t_minimap_vars	map_data;
	t_sprite		*sprite;
	double			angle_0;
	double			angle_1;
	pthread_mutex_t	lock;
	t_threads		threads[4];
	t_data			*data;
}		t_cub;

void	ft_fractol_init(t_cub *cube);

// hooks
void	loop_fun(void *param);
void	my_keyhook(mlx_key_data_t keydata, void *param);
// end hooks
//minimap
void	draw_per(t_cub *cube);
int		create_rgba(int r, int g, int b, int a);

//raycasting
void	DDA(t_cub *cube, double X0, double Y0, double X1, double Y1);
int		has_wall(t_cub *cube, double x1, double y1, int is);
t_vars	draw_line(t_cub *cube, double angle, int is);
void	draw_lines(t_cub *cube, int is);
double	normalizeAngle(double angle);

void	ft_draw_player(t_cub *cube);
void	my_mousehook(mouse_key_t button, action_t action,
			modifier_key_t mods, void *param);
///
t_doors	*ft_lstnew_doors(int x, int y);
void	ft_lstaddback_doors(t_doors **head, t_doors *node);
int		ft_doors_size(t_doors *head);
void	ft_lstdel_doors(t_doors *lst);
void	ft_lstclear_doors(t_doors **lst);

//wall 
void	draw_lines_3d(t_cub *cube);
uint32_t	get_pixel_color(mlx_texture_t *texture, int x, int y);

// sky floor
void	ft_draw_sky_floor_1(t_cub *cube, int i,
			int sky_end_y, int floor_start_y);
void	ft_draw_sky_floor(t_cub *cube);

//Draw 
void	draw_cube(t_cub *cube, int x, int y, int color);
void	draw_all_black(t_cub *cube);

// raycastin
void	update_player(t_cub *cube);
void	handle_mouse(t_cub *cube);

//extra
double	distance_between_points(double x1, double y1, double x2, double y2);
void	ft_error(void);
void	ft_free_data(t_cub *cube);
void	malloc_error(void);
double	deg2rad(double degrees);

// iswall
int		is_it_a_wall(double x, double y, t_cub *cube);

// player
t_player	*init_player(t_cub *cube);

#endif
