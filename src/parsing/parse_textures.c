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

int	parse_textures(t_game *game, char *line)
{
	char	*trimmed;
	char	**split;
	
	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (1);
	
	split = ft_split(trimmed, ' ');
	if (!split || !split[0] || !split[1])
	{
		free(trimmed);
		free_split(split);
		return (0); // Not a texture line, continue
	}
	
	// Load PNG textures based on direction
	if (ft_strncmp(split[0], "NO", 3) == 0)
	{
		printf("Trying to load North texture: '%s'\n", split[1]);
		mlx_texture_t *texture = mlx_load_png(split[1]);
		if (texture)
		{
			game->map->textures.north = texture;
			printf("Texture NO: %s loaded successfully\n", split[1]);
		}
		else
			printf("Texture NO: %s failed to load\n", split[1]);
	}
	else if (ft_strncmp(split[0], "SO", 3) == 0)
	{
		mlx_texture_t *texture = mlx_load_png(split[1]);
		if (texture)
		{
			game->map->textures.south = texture;
			printf("Texture SO: %s loaded successfully\n", split[1]);
		}
		else
			printf("Texture SO: %s failed to load\n", split[1]);
	}
	else if (ft_strncmp(split[0], "WE", 3) == 0)
	{
		mlx_texture_t *texture = mlx_load_png(split[1]);
		if (texture)
		{
			game->map->textures.west = texture;
			printf("Texture WE: %s loaded successfully\n", split[1]);
		}
		else
			printf("Texture WE: %s failed to load\n", split[1]);
	}
	else if (ft_strncmp(split[0], "EA", 3) == 0)
	{
		mlx_texture_t *texture = mlx_load_png(split[1]);
		if (texture)
		{
			game->map->textures.east = texture;
			printf("Texture EA: %s loaded successfully\n", split[1]);
		}
		else
			printf("Texture EA: %s failed to load\n", split[1]);
	}
	else if (ft_strncmp(split[0], "DO", 3) == 0)
	{
		mlx_texture_t *texture = mlx_load_png(split[1]);
		if (texture)
		{
			game->map->textures.door = texture;
			printf("Texture DO: %s loaded successfully\n", split[1]);
		}
		else
			printf("Texture DO: %s failed to load\n", split[1]);
	}
	
	free(trimmed);
	free_split(split);
	return (0);
}
