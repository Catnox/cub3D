/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_closed.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/29 13:31:34 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Check if a position is valid empty space that needs validation
 * 
 * @param map Map structure
 * @param x X coordinate
 * @param y Y coordinate
 * @return 1 if position is valid empty space, 0 otherwise
 */
static int	is_valid_empty_space(t_map *map, int x, int y)
{
	if (y < 0 || y >= map->height || x < 0 || x >= map->width)
		return (0);
	if (!map->grid[y][x])
		return (0);
	return (map->grid[y][x] == '0' || map->grid[y][x] == 'N'
		|| map->grid[y][x] == 'S' || map->grid[y][x] == 'E'
		|| map->grid[y][x] == 'W');
}

/**
 * Check if a position has a wall or boundary around it
 * 
 * @param map Map structure
 * @param x X coordinate
 * @param y Y coordinate
 * @return 1 if position is surrounded by walls, 0 if has opening
 */
static int	check_position_surrounded(t_map *map, int x, int y)
{
	if (y == 0 || y == map->height - 1)
		return (0);
	if (x == 0 || x >= map->width - 1 || !map->grid[y][x + 1]
		|| map->grid[y][x + 1] == ' ')
		return (0);
	if (!map->grid[y][x - 1] || map->grid[y][x - 1] == ' ')
		return (0);
	if (!map->grid[y - 1][x] || map->grid[y - 1][x] == ' ')
		return (0);
	if (!map->grid[y + 1][x] || map->grid[y + 1][x] == ' ')
		return (0);
	return (1);
}

/**
 * Validate that the map is properly closed (surrounded by walls)
 * 
 * Checks that all empty spaces (0, N, S, E, W) are surrounded by walls
 * and cannot lead to the edges of the map.
 * 
 * @param map Map structure to validate
 * @return 0 if map is closed, 1 if map has openings
 */
int	validate_map_closed(t_map *map)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width && map->grid[y][x])
		{
			if (is_valid_empty_space(map, x, y))
			{
				if (!check_position_surrounded(map, x, y))
				{
					error_exit("Map is not closed - spaces must be surrounded by walls");
					return (1);
				}
			}
			x++;
		}
		y++;
	}
	return (0);
}
