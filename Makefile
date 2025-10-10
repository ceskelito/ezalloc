NAME	= libezalloc.a
AR		= ar -rcs
CC		= gcc
RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= -I. -Iinternal -Iinternal/include -Itest

FILES	= 	ezalloc \
			internal/internal_ezalloc

SRCS	= $(addsuffix .c, $(FILES))
OBJS	= $(addsuffix .o, $(FILES))

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@

$(NAME): $(OBJS)
	$(AR) $@ $^
	
clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all
