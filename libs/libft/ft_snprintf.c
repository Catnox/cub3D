/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 09:56:25 by radubos           #+#    #+#             */
/*   Updated: 2025/06/26 22:03:46 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	args;
	int		len;

	va_start(args, format);
	len = vsnprintf(str, size, format, args);
	va_end(args);
	return (len);
}
