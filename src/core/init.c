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

/**
 * Initialize game state and MLX42 graphics library
 * 
 * Sets up the basic game state variables and initializes the MLX42 graphics
 * library with a window and main image buffer for rendering.
 * 
 * @param game Pointer to the main game structure
 * @return 0 on success, 1 on failure
 */
int init_game_state_n_MLX(t_game *game)
{
	if (!game)
		return (1);
	game->is_running = 1;
	game->game_over = 0;

	game->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return (error_exit("Failed to initialize MLX42"), 1);
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img)
		return (error_exit("Failed to create image"), 1);
	return (0);
}

/**
 * Initialize player statistics and attributes
 * 
 * Sets up the initial player statistics including health points, ammunition,
 * score, and collected keys. All values are set to their default starting
 * values for a new game.
 * 
 * @param game Pointer to the main game structure containing player data
 * @return 0 on success, 1 if game or player is NULL
 */
int init_player_stats(t_game *game)
{
	if (!game || !game->player)
		return (1);
	game->player->max_hp = 100;
	game->player->hp = game->player->max_hp;
	game->player->max_ammo = 100;
	game->player->ammo = game->player->max_ammo;
	game->player->score = 0;
	game->player->keys = 0;
	return (0);
}

/**
 * Master initialization function for the entire game
 * 
 * This function orchestrates the complete initialization process of the cub3D game.
 * It performs the following steps in order:
 * 1. Clears the game structure memory
 * 2. Initializes MLX42 graphics system and game state
 * 3. Parses the .cub map file for textures, colors, and map data
 * 4. Locates the player's starting position on the map
 * 5. Sets up initial player statistics (HP, ammo, score, keys)
 * 6. Displays the main image buffer on the window
 * 7. Initializes interactive doors system
 * 8. Sets up the HUD (Heads-Up Display) interface
 * 9. Initializes weapon system and animations
 * 10. Spawns and configures enemy entities
 * 
 * @param game Pointer to the main game structure to initialize
 * @param map_file Path to the .cub map file to load
 * @return 0 on successful initialization, 1 on any failure
 */
int	init_game(t_game *game, char *map_file)
{
	ft_memset(game, 0, sizeof(t_game));
	if (init_game_state_n_MLX(game) != 0)
		return (1);
	if (parse_cub_file(game, map_file) != 0)
		return (1);
	if (find_player_position(game) != 0)
		return (1);
	if (init_player_stats(game) != 0)
		return (1);
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_exit("Failed to display image"), 1);
	init_doors(game);
	if (!init_hud(game))
		return (error_exit("Failed to initialize HUD"), 1);
	if (!init_weapon(game))
		return (error_exit("Failed to initialize weapon"), 1);
	if (!init_enemies(game))
		return (error_exit("Failed to initialize enemies"), 1);
	game->is_running = 1;
	return (0);
}
