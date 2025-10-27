/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:28:29 by radubos           #+#    #+#             */
/*   Updated: 2024/11/08 15:13:28 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	const char	*start;
	const char	*end;
	char		*trimmed_str;

	if (!s1 || !set)
		return (NULL);
	while (*s1 && is_in_set(*s1, set))
		s1++;
	start = s1;
	end = s1 + ft_strlen(s1) - 1;
	while (end > start && is_in_set(*end, set))
		end--;
	trimmed_str = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!trimmed_str)
		return (NULL);
	ft_strlcpy(trimmed_str, start, end - start + 2);
	return (trimmed_str);
}
