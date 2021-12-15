NAME	= philo
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -MMD -MP -I $(INCDIR)
INCDIR	= ./include
SRCDIR	= ./src
SRCNAME	=	main.c \
			lib.c \
			put.c \
			thread.c
SRCS	= $(addprefix $(SRCDIR)/, $(SRCNAME))
OBJS	= $(SRCS:.c=.o)
DEPS	= $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

.PHONY: clean
clean:
	$(RM) -rf *.dSYM
	$(RM) -rf $(OBJS) $(DEPS)

.PHONY: fclean
fclean: clean
	$(RM) -rf $(NAME)

.PHONY: re
re: fclean all

#.PHONY: test
#test: $(NAME)
#	./test.sh

.PHONY: debug
debug: CFLAGS += -g -fsanitize=integer -fsanitize=address -fsanitize=leak -fsanitize=thread
debug: re

-include $(DEPS)
