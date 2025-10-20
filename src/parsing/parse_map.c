/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:32:40 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Copy all map lines from file lines to game map grid
 * 
 * Processes each map line by copying, padding to uniform width,
 * and validating characters. Allocates memory for each line.
 * 
 * @param game Pointer to game structure
 * @param lines Array of file lines
 * @param params Structure containing map dimensions and start position
 * @return 0 on success, 1 on failure
 */
static int	copy_map_lines(t_game *game, char **lines, t_map_params params)
{
	int	i;

	i = 0;
	while (i < params.height)
	{
		game->map->grid[i] = malloc(sizeof(char) * (params.width + 1));
		if (!game->map->grid[i])
			return (error_exit("Memory allocation failed"), 1);
		copy_and_pad_line(game->map->grid[i],
			lines[params.start + i], params.width);
		if (validate_map_line(game->map->grid[i], params.width) != 0)
			return (1);
		i++;
	}
	game->map->grid[params.height] = NULL;
	return (0);
}

/**
 * Parse the map section from .cub file lines
 * 
 * The resulting map grid has uniform width (padded with spaces)
 * and contains only validated characters.
 * 
 * @param game Pointer to the main game structure
 * @param lines Array of all lines from the .cub file
 * @return 0 on successful parsing, 1 on any error
 */
int	parse_map(t_game *game, char **lines)
{
	t_map_params	params;

	params.start = find_map_start(lines);
	if (params.start == -1)
		return (error_exit("No map found in file"), 1);
	params.height = count_map_lines(lines, params.start);
	if (params.height == 0)
		return (error_exit("Empty map"), 1);
	params.width = get_max_line_width(lines, params.start, params.height);
	if (allocate_map_grid(game, params.height, params.width) != 0)
		return (1);
	if (copy_map_lines(game, lines, params) != 0)
		return (1);
	printf("Map parsed: %dx%d\n", params.width, params.height);
	return (0);
}
