CC		= cc
CFLAGS	=   -pthread  -Wall -Wextra -Werror  -Wunreachable-code -Ofast
RM		= rm -rf
SRCS	= ./Mandatory/main.c 	./Mandatory/Parsing/parsing.c	\
		  ./Mandatory/Parsing/init/ft_get_init.c		./Mandatory/Parsing/init/ft_reading_init.c\
		  ./Mandatory/Parsing/utils/file_1.c ./Mandatory/Parsing/utils/file_2.c	./Mandatory/Parsing/utils/ft_split.c	./Mandatory/Parsing/utils/get_next_line.c	./Mandatory/Parsing/utils/get_next_line_utils.c\
		  ./Mandatory/Parsing/error/error_1.c ./Mandatory/Parsing/utils/ft_colors.c ./Mandatory/Parsing/utils/ft_map.c ./Mandatory/Parsing/utils/ft_map_1.c ./Mandatory/Parsing/utils/ft_map_2.c\
		  ./Mandatory/Raycasting.c ./Mandatory/Wall.c ./Mandatory/SkyFloor.c ./Mandatory/Draw.c ./Mandatory/Extra.c ./Mandatory/Player.c ./Mandatory/isWall.c ./Mandatory/DrawHero.c ./Mandatory/DrawVer.c ./Mandatory/Helper.c

SRCS_BONUS	= ./Bonus/main.c ./Bonus/Parsing/parsing.c ./Bonus/Parsing/parsing_1.c\
		  ./Bonus/Parsing/init/ft_get_init.c	./Bonus/Parsing/init/ft_reading_init.c\
		  ./Bonus/Parsing/utils/file_1.c ./Bonus/Parsing/utils/file_2.c \
		  ./Bonus/Parsing/utils/ft_map.c	./Bonus/Parsing/utils/ft_split.c	\
		  ./Bonus/Parsing/utils/ft_map_2.c ./Bonus/Parsing/utils/ft_colors.c \
		  ./Bonus/Parsing/utils/get_next_line.c	./Bonus/Parsing/utils/get_next_line_utils.c\
		  ./Bonus/Parsing/error/error_1.c  ./Bonus/recasting/build_raycasting/recasting.c \
		  ./Bonus/recasting/build_raycasting/recasting_1.c ./Bonus/recasting/build_raycasting/recasting_2.c \
		  ./Bonus/recasting/build_raycasting/recasting_3.c ./Bonus/recasting/build_raycasting/recasting_4.c  \
		  ./Bonus/recasting/build_raycasting/recasting_5.c ./Bonus/recasting/build_raycasting/recasting_6.c\
		  ./Bonus/recasting/build_raycasting/recasting_7.c ./Bonus/recasting/build_raycasting/recasting_8.c \
		  ./Bonus/recasting/build_raycasting/recasting_9.c ./Bonus/recasting/build_raycasting/recasting_10.c\
		  ./Bonus/recasting/build_raycasting/recasting_11.c\
		  ./Bonus/recasting/mini_map/ft_mini_map.c ./Bonus/recasting/mini_map/ft_mini_map_1.c \
		  ./Bonus/recasting/mini_map/ft_mini_map_2.c\
		  ./Bonus/recasting/raycasting.c ./Bonus/recasting/raycasting_1.c\

OBJS	= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)
NAME	= cub3D
NAME_BONUS	= cub3D_bonus

all: $(NAME)

%.o: %.c ./parsing.h ./recasting.h
	$(CC) $(CFLAGS) -c $< -o $@
#-lmlx -framework OpenGL -framework AppKit
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) ~/Desktop/MLX42/build/libmlx42.a -lglfw -L/Users/$(USER)/.brew/opt/glfw/lib -pthread -lm -g -o $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) ~/Desktop/MLX42/build/libmlx42.a -lglfw -L/Users/$(USER)/.brew/opt/glfw/lib -pthread -lm -g -o $@

clean:
	$(RM) $(OBJS) $(OBJS_BONUS)


fclean: clean
	$(RM) $(NAME) $(NAME_BONUS)
re:fclean all
