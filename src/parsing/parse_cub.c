/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 13:02:32 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Validate that all required elements are present
 * 
 * Checks that all 4 textures (NO, SO, WE, EA) and both colors (F, C)
 * have been successfully parsed and are available.
 * 
 * @param game Pointer to game structure
 * @return 0 if all elements present, 1 if missing elements
 */
static int	validate_required_elements(t_game *game)
{
	if (!game->map->textures.north)
		return (error_exit("Missing north texture (NO)"), 1);
	if (!game->map->textures.south)
		return (error_exit("Missing south texture (SO)"), 1);
	if (!game->map->textures.west)
		return (error_exit("Missing west texture (WE)"), 1);
	if (!game->map->textures.east)
		return (error_exit("Missing east texture (EA)"), 1);
	if (game->map->floor_color.r == -1)
		return (error_exit("Missing floor color (F)"), 1);
	if (game->map->ceiling_color.r == -1)
		return (error_exit("Missing ceiling color (C)"), 1);
	return (0);
}

static int	allocate_game_structures(t_game *game)
{
	game->map = ft_calloc(1, sizeof(t_map));
	game->player = ft_calloc(1, sizeof(t_player));
	game->textures = ft_calloc(1, sizeof(t_texture));
	if (!game->map || !game->player || !game->textures)
	{
		if (game->map)
			free(game->map);
		if (game->player)
			free(game->player);
		if (game->textures)
			free(game->textures);
		return (1);
	}
	game->map->floor_color.r = -1;
	game->map->ceiling_color.r = -1;
	return (0);
}

/**
 * Parse and load a .cub map file for the cub3D game
 * 
 * This function is the main entry point for parsing .cub files and performs
 * a complete validation and loading process. The .cub file format contains:
 * - Texture paths for the four wall directions (NO, SO, EA, WE)
 * - Floor and ceiling colors (F, C) in RGB format
 * - A 2D map layout using characters (0=floor, 1=wall, 
 * N/S/E/W=player start, etc.)
 * 
 * @param game Pointer to the main game structure to populate
 * @param filename Path to the .cub file to parse
 * @return 0 on successful parsing and validation, 1 on any error
 */
int	parse_cub_file(t_game *game, char *filename)
{
	char	**lines;
	int		i;

	if (validate_cub_extension(filename) != 0)
		return (1);
	lines = open_and_read_file(filename);
	if (!lines)
		return (1);
	if (allocate_game_structures(game) != 0)
		return (free_lines(lines), 1);
	i = 0;
	while (lines[i])
	{
		if (parse_textures(game, lines[i]) != 0
			|| parse_colors(game, lines[i]) != 0)
			return (free_lines(lines), 1);
		i++;
	}
	if (parse_map(game, lines) != 0)
		return (free_lines(lines), 1);
	free_lines(lines);
	if (validate_required_elements(game) != 0)
		return (1);
	return (validate_map(game->map));
}
