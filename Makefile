NAME	= libezalloc.a
AR		= ar -rcs
CC		= gcc
RM		= rm -f
MKDIR	= mkdir -p

CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= -Iinclude -Iinclude/internal
SDIR 	= srcs
ODIR 	= objs

FILES	= 	ezalloc \
			allocation_handler_internal \
			group_handler_internal
SRCS	= $(addprefix $(SDIR)/,$(addsuffix .c, $(FILES)))
OBJS	= $(addprefix $(ODIR)/,$(addsuffix .o, $(FILES)))

all: $(NAME)

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@
	
$(ODIR):
	$(MKDIR) $@

$(NAME): $(OBJS)
	$(AR) $@ $^
	
clean:
	$(RM) $(OBJS)
	$(RM) -r $(ODIR)
	
fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all
