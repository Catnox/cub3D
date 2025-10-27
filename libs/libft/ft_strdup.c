/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 11:45:18 by radubos           #+#    #+#             */
/*   Updated: 2025/05/26 16:32:22 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*ptr;
	size_t	len;

	if (!s)
		return (NULL);
	len = ft_strlen(s) + 1;
	ptr = malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s, len);
	return (ptr);
}
