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

void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	
	// Cleanup HUD
	cleanup_hud(game);
	
	// Cleanup weapon
	cleanup_weapon(game);
	
	// Cleanup enemies
	cleanup_enemies(game);
	
	// Free textures
	if (game->textures)
	{
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
	
	// Free map
	if (game->map)
	{
		if (game->map->grid)
			free_split(game->map->grid);
		if (game->map->doors)
			free(game->map->doors);
		free(game->map);
	}
	
	// Free player
	if (game->player)
		free(game->player);
	
	// Free MLX
	if (game->img)
		mlx_delete_image(game->mlx, game->img);
	if (game->mlx)
		mlx_terminate(game->mlx);
}
