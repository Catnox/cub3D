/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_next.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:51:17 by radubos           #+#    #+#             */
/*   Updated: 2024/11/26 12:34:22 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	check_next(const char *format, va_list args)
{
	int	count;

	count = 0;
	if (!format)
		return (-1);
	if (*format == 'c')
		count += ft_putchar(va_arg(args, int));
	else if (*format == 's')
		count += ft_putstr(va_arg(args, char *));
	else if (*format == 'p')
		count += ft_putadr(va_arg(args, unsigned long));
	else if (*format == 'd' || *format == 'i')
		count += ft_putnbr(va_arg(args, int));
	else if (*format == 'u')
		count += ft_putuint(va_arg(args, int));
	else if (*format == 'x')
		count += ft_puthexa(va_arg(args, int), 'x');
	else if (*format == 'X')
		count += ft_puthexa(va_arg(args, int), 'X');
	else if (*format == '%')
		count += ft_putchar('%');
	else
		return (-1);
	return (count);
}
