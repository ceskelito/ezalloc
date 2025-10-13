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
			ezgalloc \
			allocation_handler_internal \
			group_handler_internal
SRCS	= $(addprefix $(SDIR)/,$(addsuffix .c, $(FILES)))
OBJS	= $(addprefix $(ODIR)/,$(addsuffix .o, $(FILES)))

TEST_BIN	= test
TEST_SRC = test.c

all: $(NAME)

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@
	
$(ODIR):
	$(MKDIR) $@

$(NAME): $(OBJS)
	$(AR) $@ $^

$(TEST_BIN): $(NAME) $(TEST_SRC)
	$(CC) $(CFLAGS) $(IFLAGS) $(TEST_SRC) -L. -lezalloc -o $(TEST_BIN)

run-test: $(TEST_BIN)
	valgrind $(VAL_FLAGS) ./$(TEST_BIN)

run-test-file: $(TEST_BIN)
	vlagrind $(VAL_FLAGS) ./$(TEST_BIN) 2>valout.py
	
clean:
	$(RM) $(OBJS)
	$(RM) -r $(ODIR)
	$(RM) $(TEST_BIN)
	
fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all run-test
