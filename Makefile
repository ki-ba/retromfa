NAME = retromfa

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC = main.c
INCS = retromfa.h

INC_DIR = inc/
OBJ_DIR = .obj/
SRC_DIR = src/

OBJ = $(addprefix $(OBJ_DIR), $(SRC:%.c=%.o))


.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@


$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
