CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g   #-fsanitize=address
RM		= rm -rf
SRCS	= main.c 	./Parsing/parsing.c	\
		  ./Parsing/init/ft_get_init.c		./Parsing/init/ft_reading_init.c\
		  ./Parsing/utils/file_1.c	./Parsing/utils/ft_split.c	./Parsing/utils/get_next_line.c	./Parsing/utils/get_next_line_utils.c\
		  ./Parsing/error/error_1.c \
		  recasting/recasting.c 

OBJS	= $(SRCS:.c=.o)
NAME	= app

all: $(NAME)

%.o: %.c parsing.h
	$(CC) $(CFLAGS) -c $< -o $@ 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)  -lmlx -framework OpenGL -framework AppKit -o $@

clean:
	$(RM) $(OBJS) $(OBJSB)

fclean: clean
	$(RM) $(NAME)

re:fclean all
