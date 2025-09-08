/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 13:00:42 by radubos           #+#    #+#             */
/*   Updated: 2024/11/12 10:12:15 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(char const *s, char c)
{
	size_t	count;
	int		word;

	count = 0;
	word = 1;
	while (*s)
	{
		if (*s && *s == c)
			word = 1;
		else if (*s && *s != c && word)
		{
			word = 0;
			count++;
		}
		s++;
	}
	return (count);
}

static size_t	word_size(char const *s, char c)
{
	size_t	size;

	size = 0;
	while (s[size] != '\0' && s[size] != c)
		size++;
	return (size);
}

static void	free_split(char **arr, size_t size)
{
	while (size--)
		free(arr[size]);
	free(arr);
}

static int	fill_words(char **arr, const char *s, char c)
{
	size_t	size;
	size_t	i;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			size = word_size(s, c);
			arr[i] = malloc(sizeof(char) * (size + 1));
			if (!arr[i])
			{
				free_split(arr, i);
				return (0);
			}
			ft_strlcpy(arr[i++], s, size + 1);
			s += size;
		}
	}
	arr[i] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = malloc(sizeof(char *) * (word_count(s, c) + 1));
	if (!arr)
		return (NULL);
	if (!fill_words(arr, s, c))
		return (NULL);
	return (arr);
}
