/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/20 12:45:04 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Copy and pad a single map line to the specified width
 * 
 * Copies characters from the source line and pads with spaces
 * if the line is shorter than the maximum width. This ensures
 * all map lines have uniform width.
 * 
 * @param dest Destination buffer (must be allocated)
 * @param src Source line to copy
 * @param max_width Target width for the line
 */
void	copy_and_pad_line(char *dest, char *src, int max_width)
{
	int	line_len;
	int	k;

	line_len = ft_strlen(src);
	k = 0;
	while (k < line_len && k < max_width)
	{
		dest[k] = src[k];
		k++;
	}
	while (k < max_width)
	{
		dest[k] = ' ';
		k++;
	}
	dest[max_width] = '\0';
}

/**
 * Validate all characters in a map line
 * 
 * Checks each character in the line to ensure it's a valid
 * map character according to the game rules.
 * 
 * @param line The map line to validate
 * @param width Width of the line to check
 * @return 0 if valid, 1 if invalid character found
 */
int	validate_map_line(char *line, int width)
{
	int	j;

	j = 0;
	while (j < width)
	{
		if (!is_valid_map_char(line[j]))
			return (error_exit("Invalid character in map"), 1);
		j++;
	}
	return (0);
}

int	allocate_map_grid(t_game *game, int height, int width)
{
	int	i;

	game->map->height = height;
	game->map->width = width;
	game->map->grid = malloc(sizeof(char *) * (height + 1));
	if (!game->map->grid)
		return (error_exit("Memory allocation failed"), 1);
	i = 0;
	while (i <= height)
	{
		game->map->grid[i] = NULL;
		i++;
	}
	return (0);
}
