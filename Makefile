NAME = retromfa

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC = main.c
INCS = retromfa.h

INC_DIR = inc/
OBJ_DIR = .obj/
SRC_DIR = src/
MLX_DIR = minilibx-linux/

MLX_LIB = $(MLX_DIR)libmlx.a
LIB_FLAG = -Lminilibx-linux -lmlx -lXext -lX11 -lm -lz


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
re: fclean
	$(MAKE) all

.PHONY: mlx
mlx:
	@$(MAKE) -C $(MLX_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ) | $(OBJ_DIR) mlx
	$(CC) $(CFLAGS) $(LIB_FLAG) $(OBJ) $(MLX_LIB) -o $@


$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC) | $(OBJ_DIR)
	$(CC) -I$(INC_DIR) -I$(MLX_DIR) $(CFLAGS) -c $< -o $@
