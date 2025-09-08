/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putadr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:01:08 by radubos           #+#    #+#             */
/*   Updated: 2024/11/20 17:35:41 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putadr(unsigned long n)
{
	char const		*base;
	char			res[17];
	int				i;
	int				count;

	base = "0123456789abcdef";
	i = 16;
	count = 0;
	if (n == 0)
		return (count += ft_putstr("(nil)"));
	res[i] = '\0';
	while (n > 0)
	{
		res[--i] = base[(n % 16)];
		n /= 16;
	}
	count += ft_putstr("0x");
	count += ft_putstr(&res[i]);
	return (count);
}
