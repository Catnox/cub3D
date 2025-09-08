/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	init_game(t_game *game, char *map_file)
{
	ft_memset(game, 0, sizeof(t_game));
	
	// Initialize game state
	game->is_running = 1;
	game->game_over = 0;
	
	// Initialize MLX42
	game->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return (error_exit("Failed to initialize MLX42"), 1);
	
	// Create main image
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img)
		return (error_exit("Failed to create image"), 1);
	
	// Parse map file (this allocates game->player)
	if (parse_cub_file(game, map_file) != 0)
		return (1);
	
	// Find and set player position from map
	if (find_player_position(game) != 0)
		return (1);
	
	// Initialize player stats after player is allocated and positioned
	game->player->max_hp = 100;
	game->player->hp = game->player->max_hp;
	game->player->max_ammo = 100;
	game->player->ammo = game->player->max_ammo;
	game->player->score = 0;
	game->player->keys = 0;
	
	// Display image
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_exit("Failed to display image"), 1);
	
	// Initialize doors
	init_doors(game);
	
	// Initialize HUD
	if (!init_hud(game))
		return (error_exit("Failed to initialize HUD"), 1);
	
	// Initialize weapon
	if (!init_weapon(game))
		return (error_exit("Failed to initialize weapon"), 1);
	
	// Initialize enemies
	if (!init_enemies(game))
		return (error_exit("Failed to initialize enemies"), 1);
	
	game->is_running = 1;
	return (0);
}
