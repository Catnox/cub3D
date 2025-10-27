/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:11:22 by radubos           #+#    #+#             */
/*   Updated: 2024/11/08 14:29:34 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;
	char	value;

	ptr = (char *)s;
	value = (char)c;
	while (*ptr)
	{
		if (*ptr == value)
			return (ptr);
		ptr++;
	}
	if (value == '\0')
		return (ptr);
	return (NULL);
}
