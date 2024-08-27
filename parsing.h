#ifndef PARSING_H
# define PARSING_H


# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>

#  define BUFFER_SIZE 1

typedef struct  s_init
{
	char    **file;
	int     file_lines;
	char    **map;
	char    **coordinats;
	char    **colors;
}   t_init;



typedef struct s_color
{
	int r;
	int g;
	int b;
}   t_color;

typedef struct  s_data
{
	char			**map;
	char			*no;
	char			*so;
	char			*we;
	char			*ea;
	char			p;
	int				map_row;
	int				map_cols;
	int				height;
	int				width;
	t_color			floor;
	t_color			sky;
}	t_data;


// parsing
int		ft_count_map_words(char **map);

//get next line
int		ft_strchr(char *s, int c);
char	*ft_strdup(char *s1);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_calloc(int count);
int		ft_strlen(char *str);
char	*get_next_line(int fd);

//split
char	**ft_split(char *s, char c, char p);
int		ft_words_counter(char *str, char c, char p);

//file 1
int		ft_atoi(char *str);
int		ft_strncmp(char *s1, char *s2, size_t n);
void	ft_putstr_fd(char *str, int fd);
void	free_double_arr(char **tmp);

//ft get init
int		ft_count_arrgs_in_file(t_init *init, char *key);
void	ft_check_file(t_init *init);
int		ft_check_2_cords_init(char **cords, t_data *data);

//ft reading init

int		ft_get_data_init(t_init *init, t_data *data);
int		ft_get_coordinats_init(t_init *init, t_data *datainit);
int		ft_get_colors_init(t_init *init);
int		ft_get_data(t_init *init, t_data *data);

//error 1

void	ft_check_color_print(t_data *data);
void	ft_print_cords_err(t_init *init);
void	ft_check_file(t_init *init);
void	ft_check_map_print(t_data *data);

// ft_colors

int		ft_count_cammas(char *ptr);
int		ft_fromdst_to_tmp(char **colors, t_color *tmp);
void	ft_count_num(int *i, int *j, char **colors);
int		ft_check_char(char c, int is);
void	ft_check_color(t_data *data);

// ft_map

int		ft_check_map(t_data *data);
void	ft_change_map(t_init *init, t_data *data);
void	ft_open_files(t_data *data);
int		ft_map_check_words(char **map);
int		check_only_space(char *str);
int		ft_get_player(t_data *data);
int		ft_check_map(t_data *data);

#endif
