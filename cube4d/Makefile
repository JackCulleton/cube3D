NAME := cube4d

CC := cc
CFLAGS := -Wall -Wextra -Werror

MLX_DIR := minilibx-linux
MLX_FLAGS := -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd

SRC := main.c user_input.c init_map.c draw_map.c ray_draw.c
OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C $(MLX_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(MLX_FLAGS) -o $(NAME)

%.o: %.c header.h
	$(CC) $(CFLAGS) -I$(MLX_DIR) -c $< -o $@

clean:
	$(MAKE) -C $(MLX_DIR) clean
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
