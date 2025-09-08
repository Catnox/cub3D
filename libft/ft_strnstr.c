/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:45:29 by radubos           #+#    #+#             */
/*   Updated: 2024/11/12 10:29:05 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isvalid(const char *big, const char *little, size_t len)
{
	while (*little && *big == *little && len)
	{
		big++;
		little++;
		len--;
	}
	if (*little == '\0')
		return (1);
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	if (!big && len == 0)
		return (NULL);
	if (*little == '\0')
		return ((char *)big);
	while (*big && len)
	{
		if (*big == *little)
		{
			if (isvalid(big, little, len))
				return ((char *)big);
		}
		big++;
		len--;
	}
	return (NULL);
}
