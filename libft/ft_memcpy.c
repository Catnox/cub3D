/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 11:46:19 by radubos           #+#    #+#             */
/*   Updated: 2024/11/12 10:26:52 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*ptr;
	const unsigned char	*src_ptr;

	if (n == 0)
		return (dest);
	if (!dest && !src)
		return (NULL);
	ptr = (unsigned char *)dest;
	src_ptr = (const unsigned char *)src;
	while (n--)
		*ptr++ = *src_ptr++;
	return (dest);
}
