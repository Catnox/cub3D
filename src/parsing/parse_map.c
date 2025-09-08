/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	find_map_start(char **lines)
{
	int	i;
	
	i = 0;
	while (lines[i])
	{
		// Skip texture and color lines
		if (ft_strncmp(lines[i], "NO ", 3) == 0 || 
			ft_strncmp(lines[i], "SO ", 3) == 0 ||
			ft_strncmp(lines[i], "WE ", 3) == 0 || 
			ft_strncmp(lines[i], "EA ", 3) == 0 ||
			ft_strncmp(lines[i], "DO ", 3) == 0 ||
			ft_strncmp(lines[i], "F ", 2) == 0 || 
			ft_strncmp(lines[i], "C ", 2) == 0 ||
			lines[i][0] == '\0')
		{
			i++;
			continue;
		}
		
		// If line contains map characters, it's the start
		if (ft_strchr(lines[i], '1') || ft_strchr(lines[i], '0'))
			return (i);
		i++;
	}
	return (-1);
}

static int	get_max_line_width(char **lines, int start, int count)
{
	int	max_width;
	int	i;
	int	current_width;
	
	max_width = 0;
	i = 0;
	while (i < count)
	{
		current_width = ft_strlen(lines[start + i]);
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	return (max_width);
}

int	parse_map(t_game *game, char **lines)
{
	int	map_start;
	int	map_height;
	int	max_width;
	int	i;
	int	j;
	
	// Find where the map starts in the file
	map_start = find_map_start(lines);
	if (map_start == -1)
		return (error_exit("No map found in file"), 1);
	
	// Count map lines
	map_height = 0;
	i = map_start;
	while (lines[i])
	{
		if (ft_strchr(lines[i], '1') || ft_strchr(lines[i], '0') || 
			ft_strchr(lines[i], 'N') || ft_strchr(lines[i], 'S') ||
			ft_strchr(lines[i], 'E') || ft_strchr(lines[i], 'W') ||
			ft_strchr(lines[i], 'D'))
			map_height++;
		else if (lines[i][0] != '\0')
			break; // End of map
		i++;
	}
	
	if (map_height == 0)
		return (error_exit("Empty map"), 1);
	
	// Get maximum width
	max_width = get_max_line_width(lines, map_start, map_height);
	
	// Allocate map grid
	game->map->height = map_height;
	game->map->width = max_width;
	game->map->grid = malloc(sizeof(char *) * (map_height + 1));
	if (!game->map->grid)
		return (error_exit("Memory allocation failed"), 1);
	
	// Copy map lines and pad with spaces if needed
	i = 0;
	while (i < map_height)
	{
		game->map->grid[i] = malloc(sizeof(char) * (max_width + 1));
		if (!game->map->grid[i])
			return (error_exit("Memory allocation failed"), 1);
		
		// Copy the line
		int line_len = ft_strlen(lines[map_start + i]);
		int k = 0;
		while (k < line_len && k < max_width)
		{
			game->map->grid[i][k] = lines[map_start + i][k];
			k++;
		}
		
		// Pad with spaces if line is shorter than max_width
		while (k < max_width)
		{
			game->map->grid[i][k] = ' ';
			k++;
		}
		game->map->grid[i][max_width] = '\0';
		
		// Validate characters
		j = 0;
		while (j < max_width)
		{
			if (!is_valid_map_char(game->map->grid[i][j]))
				return (error_exit("Invalid character in map"), 1);
			j++;
		}
		i++;
	}
	game->map->grid[map_height] = NULL;
	
	printf("Map parsed: %dx%d\n", max_width, map_height);
	return (0);
}
