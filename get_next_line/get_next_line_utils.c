/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 13:04:11 by radubos           #+#    #+#             */
/*   Updated: 2024/12/27 11:03:37 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

int	ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
	{
		if (s[i++] == c)
			return (1);
	}
	return (0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	len1;
	size_t	len2;
	int		i;

	i = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = malloc(sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	if (s1)
		while (len1--)
			res[i++] = *s1++;
	if (s2)
		while (len2--)
			res[i++] = *s2++;
	res[i] = '\0';
	return (res);
}

char	*extract_line(char *tank, char c)
{
	char	*res;
	int		i;

	i = 0;
	if (!tank)
		return (NULL);
	while (tank[i] && tank[i] != c)
		i++;
	if (tank[i])
		i++;
	res = malloc(sizeof(char) * (i + 1));
	if (!res)
		return (NULL);
	res[i] = '\0';
	while (i)
	{
		i--;
		res[i] = tank[i];
	}
	return (res);
}

char	*update_tank(char *tank, char c)
{
	char	*res;
	int		i;
	int		size;

	i = 0;
	if (!tank)
		return (NULL);
	while (tank[i] && tank[i] != c)
		i++;
	if (tank[i])
		i++;
	size = ft_strlen(tank);
	res = malloc(sizeof(char) * (size - i + 1));
	if (!res)
		return (NULL);
	res[size - i] = '\0';
	while (size - i)
	{
		size--;
		res[size - i] = tank[size];
	}
	return (res);
}
