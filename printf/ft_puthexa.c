/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthexa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 10:15:34 by radubos           #+#    #+#             */
/*   Updated: 2024/11/26 16:45:38 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_puthexa(unsigned int n, char c)
{
	char const		*base;
	char			res[9];
	int				i;
	int				count;

	if (c == 'x')
		base = "0123456789abcdef";
	else if (c == 'X')
		base = "0123456789ABCDEF";
	i = 8;
	count = 0;
	if (n == 0)
		return (count += ft_putstr("0"));
	res[i] = '\0';
	while (n > 0)
	{
		res[--i] = base[(n % 16)];
		n /= 16;
	}
	count += ft_putstr(&res[i]);
	return (count);
}
