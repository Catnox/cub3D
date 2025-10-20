/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/20 10:56:53 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Resize the lines array when capacity is exceeded
 * 
 * Doubles the capacity of the lines array using malloc and manual copying
 * to accommodate more lines when the current capacity is reached.
 * This approach avoids realloc for better control and 42 compatibility.
 * 
 * @param lines Pointer to the current lines array
 * @param capacity Pointer to current capacity (will be doubled)
 * @param count Current number of lines in the array
 * @return Pointer to resized array, or NULL on failure
 */
char	**resize_lines_array(char **lines, int *capacity, int count)
{
	char	**new_lines;
	int		i;
	int		new_capacity;

	new_capacity = (*capacity) * 2;
	new_lines = malloc(sizeof(char *) * new_capacity);
	if (!new_lines)
	{
		free(lines);
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_lines[i] = lines[i];
		i++;
	}
	free(lines);
	*capacity = new_capacity;
	return (new_lines);
}

void	process_line(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

void	free_lines(char **lines)
{
	int	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		i++;
	}
	free(lines);
}
