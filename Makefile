NAME	= philo
CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -MMD -MP -I $(INCDIR)
INCDIR	= ./include

SRCDIR	= ./src
SRCNAME	=	main.c \
			lib.c \
			put.c \
			thread.c \
			setdata.c
SRCS	= $(addprefix $(SRCDIR)/, $(SRCNAME))

OBJDIR	= ./obj
OBJS	= $(SRCNAME:%.c=$(OBJDIR)/%.o)
DEPS	= $(OBJS:.o=.d)

.PHONY: all
all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p obj

.PHONY: clean
clean:
	$(RM) -rf $(OBJDIR) *.dSYM

.PHONY: fclean
fclean: clean
	$(RM) -f $(NAME)

.PHONY: re
re: fclean all

#.PHONY: test
#test: $(NAME)
#	./test.sh

.PHONY: debug
debug: CFLAGS += -g -fsanitize=thread
debug: re

-include $(DEPS)
