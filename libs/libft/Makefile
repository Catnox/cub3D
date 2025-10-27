# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/28 12:49:24 by radubos           #+#    #+#              #
#    Updated: 2025/05/26 16:56:49 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = ft_isprint.c     ft_putendl_fd.c  ft_strlcat.c  ft_substr.c \
ft_atoi.c     ft_itoa.c        ft_putnbr_fd.c   ft_strlcpy.c  ft_tolower.c \
ft_bzero.c    ft_memchr.c      ft_putstr_fd.c   ft_strlen.c   ft_toupper.c \
ft_calloc.c   ft_memcmp.c      ft_split.c       ft_strmapi.c  ft_isalnum.c \
ft_memcpy.c   ft_strchr.c      ft_strncmp.c     ft_isalpha.c  ft_memmove.c \
ft_strdup.c   ft_strnstr.c     ft_isascii.c     ft_memset.c   ft_striteri.c \
ft_strrchr.c  ft_isdigit.c     ft_putchar_fd.c  ft_strjoin.c  ft_strtrim.c \
ft_strcmp.c	  ft_isspace.c     ft_isnumeric.c   ft_snprintf.c ft_strndup.c \
ft_strncpy.c  ft_free_tab.c    ft_strjoin_free.c ft_tablen.c ft_tabdup.c ft_atol.c \
get_next_line.c get_next_line_utils.c
BONUS_SRC = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
 ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)
AR = ar rcs
$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)
%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)
all: $(NAME)
bonus: $(OBJ) $(BONUS_OBJ)
	$(AR) $(NAME) $(OBJ) $(BONUS_OBJ)
clean:
	rm -f $(OBJ) $(BONUS_OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all
.PHONY: all clean fclean re bonus
