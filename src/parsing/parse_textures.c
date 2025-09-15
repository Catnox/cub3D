/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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
	const t_texture_map	texture_map[5] = {
		{"NO", &game->map->textures.north},
		{"SO", &game->map->textures.south},
		{"EA", &game->map->textures.east},
		{"WE", &game->map->textures.west},
		{"DO", &game->map->textures.door}
	};
	int					i;

	i = 0;
	while (i < 5)
	{
		if (ft_strncmp(identifier, texture_map[i].identifier, 3) == 0)
			return (load_texture(path, texture_map[i].texture_ptr, identifier));
		i++;
	}
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
