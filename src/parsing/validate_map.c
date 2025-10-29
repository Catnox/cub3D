/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/29 13:31:35 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Validate the map for correct characters and player count
 * 
 * Ensures the map contains only valid characters and exactly one player
 * starting position. Valid characters include:
 * - '0' for empty space
 * - '1' for walls
 * - 'N', 'S', 'E', 'W' for player start positions
 * 
 * @param map Pointer to the map structure to validate
 * @return 0 if valid, 1 if invalid with error message
 */
int	validate_map(t_map *map)
{
	int	player_count;
	int	i;
	int	j;

	if (!map->grid)
		return (error_exit("No map data"), 1);
	player_count = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width && map->grid[i][j])
		{
			if (!is_valid_map_char(map->grid[i][j]))
				return (error_exit("Invalid character in map"), 1);
			if (map->grid[i][j] == 'N' || map->grid[i][j] == 'S' ||
				map->grid[i][j] == 'E' || map->grid[i][j] == 'W')
				player_count++;
			j++;
		}
		i++;
	}
	if (player_count != 1)
		return (error_exit("Map must contain exactly one player"), 1);
	if (validate_map_closed(map) != 0)
		return (1);
	return (0);
}
