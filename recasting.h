/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recasting.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eel-ghal <eel-ghal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 22:02:26 by eel-ghal          #+#    #+#             */
/*   Updated: 2024/09/18 22:02:27 by eel-ghal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECASTING_H
# define RECASTING_H

# include <math.h>
# include <stdio.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include "./parsing.h"
# include "../MLX42/include/MLX42/MLX42.h"

//check 3D
# define WIDTH 2000
# define HEIGHT 1000
# define TILE_SIZE 64
# define WALL_STRIP_WIDTH 1
# define NUM_RAYS WIDTH

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
# define Y_CLICK 181

typedef struct s_texture_data
{
	mlx_texture_t	*texture;
	uint32_t		color;
	double			textureposx;
	int				texture_x;
	int				texture_y;
	double			texture_pos;
	double			shade;
}	t_texture_data;

typedef struct s_mouse_data
{
	int32_t	prev_xpos;
	int32_t	xpos;
	int32_t	ypos;
	double	sensitivity;
	int32_t	delta_x;
	int32_t	prev_ypos;
	int32_t	delta_y;
}	t_mouse_data;

typedef struct s_het_wall
{
	double	left;
	double	up;
	double	right;
	double	down;
	int		t_left;
	int		t_up;
	int		t_right;
	int		t_down;
}	t_het_wall;
typedef struct s_p_indecator
{
	int	indicator_length;
	int	end_x;
	int	end_y;
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;
}	t_p_indecator;

typedef struct s_map_background
{
	int	y;
	int	x;
	int	sx;
	int	sy;
	int	dx;
	int	dy;
}	t_map_background;

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
	int		israyfacingdown;
	int		israyfacingup;
	int		israyfacingright;
	int		israyfacingleft;
	int		foundhorzwallhit;
	int		foundvertwallhit;

	double	y_intercept;
	double	x_intercept;

	double	ystep;
	double	xstep;

	double	nexthorztouchx;
	double	nexthorztouchy;

	double	horzwallhitx;
	double	horzwallhity;

	double	vertwallhitx;
	double	vertwallhity;

	double	nextverttouchx;
	double	nextverttouchy;

	double	horzhitdistance;
	double	verthitdistance;

	double	wallhitx;
	double	wallhity;

	double	distance;
	int		washitvert;

	double	angle;

	int		texture_num;
	double	texturestep;
	double	textureoffsety;
	double	wallbottompixel;
	double	walltoppixel;
	int		door;
	int		door_var;
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
	mlx_image_t		*image;
	mlx_t			*mlx;
	t_player		*player;
	mlx_texture_t	*texture[4];
	mlx_image_t		*img[4];
	mlx_texture_t	*gun[Y_CLICK];
	mlx_image_t		*gun_img[Y_CLICK];
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
	double			angle_0;
	double			angle_1;
	pthread_mutex_t	lock;
	t_threads		threads[2];
	t_data			*data;
}		t_cub;

void		ft_fractol_init(t_cub *cube);

// hooks
void		loop_fun(void *param);
void		close_window(void *param);
void		my_keyhook(mlx_key_data_t keydata, void *param);
// end hooks
//minimap
void		draw_per(t_cub *cube);
int			c_rgba(int r, int g, int b, int a);

//raycasting
int			has_wall(t_cub *cube, double x1, double y1, int is);
t_vars		draw_line(t_cub *cube, double angle, int is);
void		draw_lines(t_cub *cube, int is);
double		normalize_angle(double angle);

void		ft_draw_player(t_cub *cube);
void		my_mousehook(mouse_key_t bt, action_t ac,
				modifier_key_t md, void *pa);

//wall 
uint32_t	get_pixel_color(mlx_texture_t *texture, int x, int y);

// sky floor
void		ft_draw_sky_floor_1(t_cub *cube, int i,
				int sky_end_y, int floor_start_y);
void		ft_draw_sky_floor(t_cub *cube);

// raycastin
void		update_player(t_cub *cube);
void		handle_mouse(t_cub *cube);

//extra
double		distance_between_points(double x1, double y1, double x2, double y2);
void		ft_error(void);
void		ft_free_data(t_cub *cube);
void		malloc_error(void);
double		deg2rad(double degrees);

// iswall
int			is_it_a_wall(double x, double y, t_cub *cube);

void		update_y_press(t_cub *cube);

// draw hero
void		ft_draw_hero(t_cub *cube, t_vars *vars);
void		ft_draw_ver(t_cub *cube, t_vars *vars);

t_player	*init_player(t_cub *cube);
void		ft_draw_minimap_to_screen(t_cub *cube);
void		ft_draw_player_indecator(t_cub *cube);
void		ft_draw_map_background(t_cub *cube);
void		ft_draw_border_1(t_cub *cube, int x, int y);
int			check_drow_minimap(t_cub *cube, int dx, int dy, int is);
int			check_drow_minimap_1(t_cub *cube, int x, int y, int is);
void		ft_draw_border(t_cub *cube);
void		ft_get_player_pos(t_player *player, t_cub *cube);
void		ft_load_doors(t_cub *cube, int i, char *path);
char		*ft_itoa(int n);
uint32_t	ft_rgb(uint8_t r, uint8_t g, uint8_t b);
uint32_t	ft_shaded_color(uint32_t color, double shade);
void		ft_realse(t_cub *cube, mlx_key_data_t keydata);
void		ft_press(t_cub *cube, mlx_key_data_t keydata);
void		ft_press_1(t_cub *cube, mlx_key_data_t keydata);
int			calculate_distance(int x1, int y1, int x2, int y2);
void		draw_inside_head(t_cub *cube);
void		ft_get_texture_b(t_cub *cube, t_vars vars, int txturnm, int i);
void		ft_get_texture_m(t_cub *cube, t_vars vars, int txturnm, int i);
void		*draw_lines_3d_1(void *tmp);
t_vars		open_door(t_vars vars, t_cub *cube, double angle);
void		draw_gun_right_click(t_cub *cube);
void		*draw_lines_3d(void *tmp);
void		init_vars_1(t_vars *vars, t_cub *cube,
				double dis_pr_plan, double angle);
int			texternum(t_vars vars);
void		draw_lines_3d_m(t_cub *cube);
void		ft_check_extention(char *dst);

#endif