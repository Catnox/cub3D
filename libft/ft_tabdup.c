/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/20 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_tabdup(char **tab)
{
	char	**new_tab;
	int		count;
	int		i;

	if (!tab)
		return (NULL);
	count = ft_tablen(tab);
	new_tab = malloc(sizeof(char *) * (count + 1));
	if (!new_tab)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_tab[i] = ft_strdup(tab[i]);
		if (!new_tab[i])
		{
			ft_free_tab(new_tab);
			return (NULL);
		}
		i++;
	}
	new_tab[count] = NULL;
	return (new_tab);
}
