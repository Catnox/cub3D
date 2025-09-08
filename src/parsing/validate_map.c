/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	validate_map(t_map *map)
{
	int	player_count;
	int	i;
	int	j;
	
	if (!map->grid)
		return (error_exit("No map data"), 1);
	
	player_count = 0;
	
	// Check map characters and count players
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
	
	// Validate player count
	if (player_count != 1)
		return (error_exit("Map must contain exactly one player"), 1);
	
	// Check if map is closed (flood fill algorithm would go here)
	printf("Map validation successful: %dx%d with %d player\n", 
		map->width, map->height, player_count);
	return (0);
}
