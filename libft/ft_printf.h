/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:23:32 by radubos           #+#    #+#             */
/*   Updated: 2024/11/26 16:10:05 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

int	ft_printf(const char *format, ...)__attribute__((format(printf, 1, 2)));
int	check_next(const char *format, va_list args);
int	ft_putchar(char c);
int	ft_putstr(char *format);
int	ft_putnbr(int n);
int	ft_putadr(unsigned long n);
int	ft_putuint(unsigned int n);
int	ft_puthexa(unsigned int n, char c);