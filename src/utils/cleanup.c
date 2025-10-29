/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:08:45 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/28 16:26:33 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	clean_map(t_game *game)
{
	if (!game || !game->map)
		return ;
	if (game->map->grid)
		free_split(game->map->grid);
	free(game->map);
	game->map = NULL;
}

static void	clean_map_textures(t_game *game)
{
	if (game->map->textures.north)
	{
		mlx_delete_texture(game->map->textures.north);
		game->map->textures.north = NULL;
	}
	if (game->map->textures.south)
	{
		mlx_delete_texture(game->map->textures.south);
		game->map->textures.south = NULL;
	}
	if (game->map->textures.east)
	{
		mlx_delete_texture(game->map->textures.east);
		game->map->textures.east = NULL;
	}
	if (game->map->textures.west)
	{
		mlx_delete_texture(game->map->textures.west);
		game->map->textures.west = NULL;
	}
}

void	clean_textures(t_game *game)
{
	if (!game)
		return ;
	if (game->map)
		clean_map_textures(game);
}

/**
 * Master cleanup function for complete game shutdown
 * 
 * This function performs a comprehensive cleanup of all game resources
 * and memory to ensure no memory leaks occur when the game exits.
 * The cleanup is performed in a specific order to avoid dependency issues:
 * 
 * ORDER IS CRITICAL:
 * 1. Clean textures (must be done BEFORE mlx_terminate)
 * 2. Clean map data
 * 3. Clean player
 * 4. Terminate MLX (must be LAST)
 * 
 * @param game Pointer to the main game structure to clean up
 */
void	cleanup_game(t_game *game)
{
	if (game->map)
	{
		clean_textures(game);
		clean_map(game);
	}
	if (game->player)
	{
		free(game->player);
		game->player = NULL;
	}
	if (game->mlx)
		mlx_terminate(game->mlx);
}
