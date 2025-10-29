# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/30 15:30:00 by radubos           #+#    #+#              #
#    Updated: 2025/10/29 13:31:33 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Directories
SRCDIR = src
INCDIR = includes
LIBFTDIR = libs/libft
MLX42DIR = libs/MLX42
OBJDIR = obj

# Source files
SRCS = $(SRCDIR)/core/main.c \
       $(SRCDIR)/core/init.c \
       $(SRCDIR)/core/game_loop.c \
       $(SRCDIR)/parsing/parse_cub.c \
       $(SRCDIR)/parsing/parse_file_utils.c \
       $(SRCDIR)/parsing/parse_line_utils.c \
       $(SRCDIR)/parsing/parse_map.c \
       $(SRCDIR)/parsing/parse_map_utils.c \
       $(SRCDIR)/parsing/parse_map_validation.c \
       $(SRCDIR)/parsing/parse_textures.c \
       $(SRCDIR)/parsing/parse_player.c \
       $(SRCDIR)/parsing/parse_player_utils.c \
       $(SRCDIR)/parsing/validate_map.c \
       $(SRCDIR)/parsing/validate_map_closed.c \
       $(SRCDIR)/parsing/parse_colors.c \
       $(SRCDIR)/raycasting/raycasting.c \
       $(SRCDIR)/raycasting/dda.c \
       $(SRCDIR)/rendering/render.c \
	$(SRCDIR)/rendering/wall_utils.c \
       $(SRCDIR)/controls/keyboard.c \
       $(SRCDIR)/controls/movements.c \
       $(SRCDIR)/utils/utils.c \
       $(SRCDIR)/utils/cleanup.c \

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Libraries
LIBFT = $(LIBFTDIR)/libft.a
MLX42 = $(MLX42DIR)/build/libmlx42.a

# Includes
INCLUDES = -I$(INCDIR) -I$(LIBFTDIR) -I$(MLX42DIR)/include

# MLX42 flags (pour Linux)
MLX_FLAGS = -ldl -lglfw -pthread -lm

all: $(NAME)

$(NAME): $(LIBFT) $(MLX42) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(MLX42) $(MLX_FLAGS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "Compiling libft..."
	@make -C $(LIBFTDIR)

$(MLX42):
	@echo "Compiling MLX42..."
	@if [ ! -f "$(MLX42DIR)/CMakeLists.txt" ]; then \
		echo "Cloning MLX42..."; \
		rm -rf $(MLX42DIR); \
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX42DIR); \
	fi
	@cmake -B $(MLX42DIR)/build -S $(MLX42DIR)
	@make -C $(MLX42DIR)/build

clean:
	rm -rf $(OBJDIR)
	@make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean
	@rm -rf $(MLX42DIR)/build

re: fclean all

.PHONY: all clean fclean re
