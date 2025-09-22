/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void clean_map(t_game *game)
{
	if (!game || !game->map)
		return ;
	if (game->map->grid)
		free_split(game->map->grid);
	
	/* BONUS FEATURE DISABLED - doors */
	// if (game->map->doors)
	//     free(game->map->doors);
	
	free(game->map);
}

void clean_textures(t_game *game)
{
	if (!game || !game->textures)
		return ;
	if (game->textures->north)
		mlx_delete_texture(game->textures->north);
	if (game->textures->south)
		mlx_delete_texture(game->textures->south);
	if (game->textures->east)
		mlx_delete_texture(game->textures->east);
	if (game->textures->west)
		mlx_delete_texture(game->textures->west);
	free(game->textures);
}

/**
 * Master cleanup function for complete game shutdown
 * 
 * This function performs a comprehensive cleanup of all game resources
 * and memory to ensure no memory leaks occur when the game exits.
 * The cleanup is performed in a specific order to avoid dependency issues:
 * 
 * @param game Pointer to the main game structure to clean up
 */
void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	
	/* BONUS FEATURES DISABLED FOR NON-BONUS VERSION */
	// cleanup_hud(game);
	// cleanup_weapon(game);
	// cleanup_enemies(game);
	
	clean_textures(game);
	clean_map(game);
	if (game->player)
		free(game->player);
	if (game->img)
		mlx_delete_image(game->mlx, game->img);
	if (game->mlx)
		mlx_terminate(game->mlx);
}
