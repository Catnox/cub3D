/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 15:08:45 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/17 15:08:48 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"


void clean_map(t_game *game)
{
	printf("DEBUG: clean_map called\n");
	if (!game || !game->map)
	{
		printf("DEBUG: clean_map - game or game->map is NULL\n");
		return ;
	}
	printf("DEBUG: clean_map - freeing grid\n");
	if (game->map->grid)
		free_split(game->map->grid);
	printf("DEBUG: clean_map - freeing map structure\n");
	free(game->map);
	game->map = NULL;
	printf("DEBUG: clean_map finished\n");
}

void	clean_textures(t_game *game)
{
	printf("DEBUG: clean_textures called (game=%p, game->map=%p)\n",
           (void *)game, (void *)game->map);
    fflush(stdout);
	if (!game)
		return ;
	
	printf("DEBUG: clean_textures called\n");
	fflush(stdout);
	
	/* Clean textures from game->map->textures (where they're actually loaded) */
	if (game->map)
	{
		printf("DEBUG: game->map exists\n");
		fflush(stdout);
		if (game->map->textures.north)
		{
			printf("DEBUG: Deleting north texture\n");
			fflush(stdout);
			mlx_delete_texture(game->map->textures.north);
			game->map->textures.north = NULL;
		}
		if (game->map->textures.south)
		{
			printf("DEBUG: Deleting south texture\n");
			mlx_delete_texture(game->map->textures.south);
			game->map->textures.south = NULL;
		}
		if (game->map->textures.east)
		{
			printf("DEBUG: Deleting east texture\n");
			mlx_delete_texture(game->map->textures.east);
			game->map->textures.east = NULL;
		}
		if (game->map->textures.west)
		{
			printf("DEBUG: Deleting west texture\n");
			mlx_delete_texture(game->map->textures.west);
			game->map->textures.west = NULL;
		}
	}

	/* Free game->textures structure (allocated but unused) */
	if (game->textures)
	{
		printf("DEBUG: Freeing game->textures structure\n");
		free(game->textures);
		game->textures = NULL;
	}
	printf("DEBUG: clean_textures finished\n");
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
 * 4. Delete MLX images (must be done BEFORE mlx_terminate)
 * 5. Terminate MLX (must be LAST)
 * 
 * @param game Pointer to the main game structure to clean up
 */
void cleanup_game(t_game *game)
{
    printf("Cleaning up game...\n");

    clean_textures(game);
	clean_map(game);
    printf("Terminating MLX...\n");
    if (game->mlx)
        mlx_terminate(game->mlx);
    printf("Cleanup done!\n");
}
