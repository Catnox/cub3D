/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:13:33 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Get texture pointer based on identifier string
 * 
 * Returns a pointer to the appropriate texture field based on the
 * identifier string. This avoids complex pointer-to-pointer mapping.
 * 
 * @param game Pointer to game structure
 * @param identifier Texture identifier (NO, SO, EA, WE, DO)
 * @return Pointer to the texture location, or NULL if unknown
 */
static mlx_texture_t	**get_texture_ptr_by_id(t_game *game, char *identifier)
{
	if (ft_strncmp(identifier, "NO", 2) == 0)
		return (&game->map->textures.north);
	else if (ft_strncmp(identifier, "SO", 2) == 0)
		return (&game->map->textures.south);
	else if (ft_strncmp(identifier, "EA", 2) == 0)
		return (&game->map->textures.east);
	else if (ft_strncmp(identifier, "WE", 2) == 0)
		return (&game->map->textures.west);
	return (NULL);
}

/**
 * Load a single texture and assign it to the specified location
 * 
 * @param path Path to the texture file
 * @param texture_ptr Pointer to where the texture should be stored
 * @param id Identifier string for logging
 * @return 0 on success, 1 on failure
 */
static int	load_texture(char *path, mlx_texture_t **texture_ptr, char *id)
{
	mlx_texture_t	*texture;

	printf("Trying to load %s texture: '%s'\n", id, path);
	texture = mlx_load_png(path);
	if (texture)
	{
		*texture_ptr = texture;
		printf("Texture %s: %s loaded successfully\n", id, path);
		return (0);
	}
	printf("Texture %s: %s failed to load\n", id, path);
	return (1);
}

/**
 * Process a texture line using lookup table approach
 * 
 * @param game Pointer to game structure
 * @param identifier Texture identifier (NO, SO, EA, WE, DO)
 * @param path Path to texture file
 * @return 0 on success, 1 on failure
 */
static int	process_texture_line(t_game *game, char *identifier, char *path)
{
	mlx_texture_t	**texture_ptr;

	texture_ptr = get_texture_ptr_by_id(game, identifier);
	if (texture_ptr)
		return (load_texture(path, texture_ptr, identifier));
	return (0);
}

int	parse_textures(t_game *game, char *line)
{
	char	*trimmed;
	char	**split;
	int		result;

	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (1);
	split = ft_split(trimmed, ' ');
	if (!split || !split[0] || !split[1])
		return (free(trimmed), free_split(split), 0);
	result = process_texture_line(game, split[0], split[1]);
	free(trimmed);
	free_split(split);
	return (result);
}
