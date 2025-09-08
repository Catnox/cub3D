/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putuint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:47:46 by radubos           #+#    #+#             */
/*   Updated: 2024/11/26 10:55:08 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putuint(unsigned int n)
{
	unsigned int	num;
	int				count;
	unsigned int	max;

	max = 4294967295;
	count = 0;
	if (n < 0)
	{
		num = max - n;
		ft_putnbr(num);
	}
	else
		num = n;
	if (num > 9)
		count += ft_putnbr(num / 10);
	count += ft_putchar((num % 10) + '0');
	return (count);
}
