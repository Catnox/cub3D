/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/20 10:57:16 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	find_map_start(char **lines)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (ft_strncmp(lines[i], "NO ", 3) == 0
			|| ft_strncmp(lines[i], "SO ", 3) == 0
			|| ft_strncmp(lines[i], "WE ", 3) == 0
			|| ft_strncmp(lines[i], "EA ", 3) == 0
			|| ft_strncmp(lines[i], "DO ", 3) == 0
			|| ft_strncmp(lines[i], "F ", 2) == 0
			|| ft_strncmp(lines[i], "C ", 2) == 0
			|| lines[i][0] == '\0')
		{
			i++;
			continue ;
		}
		if (ft_strchr(lines[i], '1') || ft_strchr(lines[i], '0'))
			return (i);
		i++;
	}
	return (-1);
}

int	get_max_line_width(char **lines, int start, int count)
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
int	count_map_lines(char **lines, int map_start)
{
	int	map_height;
	int	i;

	map_height = 0;
	i = map_start;
	while (lines[i])
	{
		if (ft_strchr(lines[i], '1') || ft_strchr(lines[i], '0')
			|| ft_strchr(lines[i], 'N') || ft_strchr(lines[i], 'S')
			|| ft_strchr(lines[i], 'E') || ft_strchr(lines[i], 'W')
			|| ft_strchr(lines[i], 'D'))
			map_height++;
		else if (lines[i][0] != '\0')
			break ;
		i++;
	}
	return (map_height);
}
