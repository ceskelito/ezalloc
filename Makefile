NAME	= libezalloc.a
AR		= ar -rcs
CC		= gcc
RM		= rm -f
MKDIR	= mkdir -p

CFLAGS	= -Wall -Wextra -Werror
IFLAGS	= -Iinclude -Iinclude/internal
VAL_FLAGS = --leak-check=full --track-origins=yes --show-leak-kinds=all
SDIR 	= srcs
ODIR 	= objs

FILES	= 	ezalloc \
			ezalloc_helper \
			ezgalloc \
			ezgalloc_helper \
			global_allocation_handler \
			groups_handler \
			helpers \
			error_message_handler \
			strings \
			memory

SRCS	= $(addsuffix .c, $(FILES))
OBJS	= $(addprefix $(ODIR)/,$(addsuffix .o, $(FILES)))

TEST_BIN = test
TEST_SRC = test.c

vpath %.c $(SDIR):$(SDIR)/internal

all: $(NAME)

$(ODIR)/%.o: %.c | $(ODIR)
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@
	
$(ODIR):
	$(MKDIR) $@

$(NAME): $(OBJS)
	$(AR) $@ $^

$(TEST_BIN): $(NAME) $(TEST_SRC)
	$(CC) $(CFLAGS) $(IFLAGS) $(TEST_SRC) -L. -lezalloc -o $(TEST_BIN)

run-test: $(TEST_BIN)
	valgrind $(VAL_FLAGS) ./$(TEST_BIN)

clean:
	$(RM) $(OBJS)
	$(RM) -r $(ODIR)
	$(RM) $(TEST_BIN)
	
fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all run-test
