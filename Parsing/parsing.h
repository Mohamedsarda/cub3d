#ifndef PARSING_H
# define PARSING_H


# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>

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
    char    **map;
    char    **coordinats;
    t_color floor;
    t_color sky;
}   t_data;


//get next line
int		ft_strchr(char *s, int c);
char	*ft_strdup(char *s1);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_calloc(int count);
int		ft_strlen(char *str);
char	*get_next_line(int fd);

//split
char	**ft_split(char const *s, char c, char p);

#endif