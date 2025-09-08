/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:44:49 by radubos           #+#    #+#             */
/*   Updated: 2024/11/08 14:31:30 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last;
	char		value;

	last = NULL;
	value = (char)c;
	while (*s)
	{
		if (*s == value)
			last = s;
		s++;
	}
	if (value == '\0')
		return ((char *)s);
	return ((char *)last);
}
