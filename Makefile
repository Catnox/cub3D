# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/30 15:30:00 by radubos           #+#    #+#              #
#    Updated: 2025/08/30 15:30:00 by radubos          ###   ########.fr        #
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
SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/parsing/parse_file.c \
       $(SRCDIR)/parsing/parse_map.c \
       $(SRCDIR)/parsing/parse_textures.c \
       $(SRCDIR)/raycasting/raycasting.c \
       $(SRCDIR)/raycasting/dda.c \
       $(SRCDIR)/rendering/render.c \
       $(SRCDIR)/rendering/textures.c \
       $(SRCDIR)/controls/movements.c \
       $(SRCDIR)/controls/rotation.c \
       $(SRCDIR)/utils/utils.c \
       $(SRCDIR)/utils/cleanup.c

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Libraries
LIBFT = $(LIBFTDIR)/libft.a
MLX42 = $(MLX42DIR)/build/libmlx42.a

# Includes
INCLUDES = -I$(INCDIR) -I$(LIBFTDIR) -I$(MLX42DIR)/include

# MLX42 flags (pour macOS)
MLX_FLAGS = -framework Cocoa -framework OpenGL -framework IOKit -lglfw

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
	@if [ ! -d "$(MLX42DIR)" ]; then \
		echo "Cloning MLX42..."; \
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