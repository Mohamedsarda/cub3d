CC		= cc
CFLAGS	=   -pthread  -Wall -Wextra -Werror  -Wunreachable-code -Ofast# -g  -fsanitize=address
RM		= rm -rf
SRCS	= main.c 	./Parsing/parsing.c	\
		  ./Parsing/init/ft_get_init.c		./Parsing/init/ft_reading_init.c\
		  ./Parsing/utils/file_1.c	./Parsing/utils/ft_split.c	./Parsing/utils/get_next_line.c	./Parsing/utils/get_next_line_utils.c\
		  ./Parsing/error/error_1.c \
		  recasting/recasting.c recasting/ft_mini_map.c recasting/raycasting.c

OBJS	= $(SRCS:.c=.o)
NAME	= cub3D_bonus

all: $(NAME)

%.o: %.c parsing.h recasting.h
	$(CC) $(CFLAGS) -c $< -o $@
#-lmlx -framework OpenGL -framework AppKit
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) ~/Desktop/MLX42/build/libmlx42.a -lglfw -L/Users/$(USER)/.brew/opt/glfw/lib -pthread -lm -g -o $@

clean:
	$(RM) $(OBJS) $(OBJSB)

fclean: clean
	$(RM) $(NAME)

re:fclean all
