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

#include "cub3d.h"

static int	find_map_start(char **lines)
{
	int	i;
	
	i = 0;
	while (lines[i])
	{
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

/**
 * Count the number of map lines starting from a given position
 * 
 * Counts consecutive lines that contain valid map characters.
 * Stops counting when encountering an empty line or invalid content.
 * 
 * @param lines Array of file lines
 * @param map_start Starting index for map lines
 * @return Number of map lines found
 */
static int	count_map_lines(char **lines, int map_start)
{
	int	map_height;
	int	i;
	
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
			break;
		i++;
	}
	return (map_height);
}

static int	allocate_map_grid(t_game *game, int height, int width)
{
	game->map->height = height;
	game->map->width = width;
	game->map->grid = malloc(sizeof(char *) * (height + 1));
	if (!game->map->grid)
		return (error_exit("Memory allocation failed"), 1);
	return (0);
}

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
static void	copy_and_pad_line(char *dest, char *src, int max_width)
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
static int	validate_map_line(char *line, int width)
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
		
		copy_and_pad_line(game->map->grid[i], lines[params.start + i], params.width);
		
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
