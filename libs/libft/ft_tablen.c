/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tablen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/20 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tablen(char **tab)
{
	int	count;

	if (!tab)
		return (0);
	count = 0;
	while (tab[count])
		count++;
	return (count);
}
