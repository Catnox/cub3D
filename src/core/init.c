/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:20:01 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Initialize game state and MLX42 graphics library
 * 
 * Sets up the basic game state variables and initializes the MLX42 graphics
 * library with a window and main image buffer for rendering.
 * 
 * @param game Pointer to the main game structure
 * @return 0 on success, 1 on failure
 */
int	init_game_state_n_mlx(t_game *game)
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
 * Master initialization function for the entire game (NON-BONUS VERSION)
 * 
 * This function initializes all necessary components for the game to run.
 * It performs the following steps in order:
 * 1. Clears the game structure memory
 * 2. Initializes MLX42 graphics system and game state
 * 3. Parses the .cub map file for textures, colors, and map data
 * 4. Locates the player's starting position on the map
 * 5. Sets up basic player data (position and direction only)
 * 6. Displays the main image buffer on the window
 * 
 * BONUS FEATURES REMOVED: doors, HUD, weapons, enemies, player stats
 * 
 * @param game Pointer to the main game structure to initialize
 * @param map_file Path to the .cub map file to load
 * @return 0 on successful initialization, 1 on any failure
 */
int	init_game(t_game *game, char *map_file)
{
	if (init_game_state_n_mlx(game) != 0)
		return (error_exit_init(game,
				"Failed to initialize game state and MLX42"));
	if (parse_cub_file(game, map_file) != 0)
		return (error_exit_init(game, "Failed to parse .cub file"));
	if (find_player_position(game) != 0)
		return (error_exit_init(game, "Failed to find player position"));
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_exit_init(game, "Failed to display image"));
	game->is_running = 1;
	return (0);
}
