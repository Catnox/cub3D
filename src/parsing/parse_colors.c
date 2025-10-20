/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:33:21 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Parse RGB color values from a comma-separated string
 * 
 * Converts a string in the format "R,G,B" (e.g., "255,128,0") into
 * individual RGB color components. Performs comprehensive validation:
 * - Splits the string by commas
 * - Ensures exactly 3 values are present (no more, no less)
 * - Validates each value is within the valid RGB range (0-255)
 * - Sets the alpha channel to full opacity (255)
 * 
 * Expected input format: "red,green,blue" where each component is 0-255
 * 
 * @param rgb_str String containing comma-separated RGB values
 * @param color Pointer to t_color structure to populate
 * @return 0 on successful parsing, 1 on format or range errors
 */
static int	parse_rgb_values(char *rgb_str, t_color *color)
{
	char	**rgb_split;
	int		r;
	int		g;
	int		b;

	rgb_split = ft_split(rgb_str, ',');
	if (!rgb_split || !rgb_split[0] || !rgb_split[1]
		|| !rgb_split[2] || rgb_split[3])
		return (free_split(rgb_split), error_exit("Invalid RGB format"), 1);
	r = ft_atoi(rgb_split[0]);
	g = ft_atoi(rgb_split[1]);
	b = ft_atoi(rgb_split[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (free_split(rgb_split),
			error_exit("RGB values must be between 0 and 255"), 1);
	color->r = r;
	color->g = g;
	color->b = b;
	color->a = 255;
	free_split(rgb_split);
	return (0);
}

/**
 * Parse floor and ceiling color definitions from .cub file lines
 * 
 * Processes lines from the .cub file that define floor and ceiling colors.
 * Recognizes two types of color definitions:
 * - "F R,G,B" for floor color (e.g., "F 220,100,0")
 * - "C R,G,B" for ceiling color (e.g., "C 225,30,0")
 * 
 * Lines that don't match the color format are ignored (not an error).
 * This allows the function to be called on all lines during parsing.
 * 
 * @param game Pointer to the main game structure containing map data
 * @param line Single line from the .cub file to process
 * @return 0 on success or ignored line, 1 on parsing errors
 */
int	parse_colors(t_game *game, char *line)
{
	char	*trimmed;

	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (1);
	if (ft_strncmp(trimmed, "F ", 2) == 0)
	{
		if (parse_rgb_values(trimmed + 2, &game->map->floor_color) != 0)
			return (free(trimmed), 1);
	}
	else if (ft_strncmp(trimmed, "C ", 2) == 0)
	{
		if (parse_rgb_values(trimmed + 2, &game->map->ceiling_color) != 0)
			return (free(trimmed), 1);
	}
	free(trimmed);
	return (0);
}
