# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/18 11:45:14 by radubos           #+#    #+#              #
#    Updated: 2024/11/26 10:53:12 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = ft_printf.c check_next.c ft_putchar.c ft_putstr.c ft_putnbr.c ft_putadr.c\
		ft_putuint.c ft_puthexa.c
OBJ = $(SRC:.c=.o)
AR = ar rcs
$(NAME) : $(OBJ)
	$(AR) $(NAME) $(OBJ)
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<
all : $(NAME)
clean : 
	rm -f $(OBJ)
fclean : clean
	rm -f $(NAME)
re : fclean all
.PHONY : all clean fclean re