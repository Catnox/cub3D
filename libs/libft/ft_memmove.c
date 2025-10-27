/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 12:35:44 by radubos           #+#    #+#             */
/*   Updated: 2024/11/08 13:56:03 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*ptr;
	const unsigned char	*src_ptr;

	if (!dest && !src)
		return (NULL);
	ptr = (unsigned char *)dest;
	src_ptr = (const unsigned char *)src;
	if (ptr > src_ptr && ptr < src_ptr + n)
	{
		ptr += n;
		src_ptr += n;
		while (n--)
			*(--ptr) = *(--src_ptr);
	}
	else
	{
		while (n--)
			*ptr++ = *src_ptr++;
	}
	return (dest);
}
