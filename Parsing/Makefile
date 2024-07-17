CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g  #-fsanitize=address
RM		= rm -rf
SRCS	= parsing.c	\
		  ./init/ft_get_init.c		./init/ft_reading_init.c\
		  ./utils/file_1.c	./utils/ft_split.c	./utils/get_next_line.c	./utils/get_next_line_utils.c\
		  ./error/error_1.c\

OBJS	= $(SRCS:.c=.o)
NAME	= app

all: $(NAME)

%.o: %.c parsing.h
	$(CC) $(CFLAGS) -c $< -o $@ 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	$(RM) $(OBJS) $(OBJSB)

fclean: clean
	$(RM) $(NAME)

re:fclean all
