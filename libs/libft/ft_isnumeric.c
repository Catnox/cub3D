/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 10:00:33 by radubos           #+#    #+#             */
/*   Updated: 2025/08/14 13:25:19 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isnumeric(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	while (*str >= '0' && *str <= '9')
	{
		if (sign == 1 && (result > (LONG_MAX - (*str - '0')) / 10))
			return (0);
		if (sign == -1 && (-result < (LONG_MIN + (*str - '0')) / 10))
			return (0);
		result = result * 10 + (*str - '0');
		str++;
	}
	return (*str == '\0');
}
