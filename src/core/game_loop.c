/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/26 16:39:32 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Main game update function called every frame
 * 
 * This function is called by MLX42's loop hook every frame and handles the
 * complete game update and rendering cycle. It processes the game in this order:
 * 1. Processes continuous input (movement, rotation)
 * 2. Renders the main 3D raycasted frame
 * 
 * @param param Void pointer to the game structure (cast from MLX42 callback)
 */
static void	game_update(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	handle_movement_rotation(game);
	render_frame(game);
}

/**
 * Initialize and start the main game loop
 * 
 * Sets up all necessary MLX42 event hooks and callbacks, then starts the
 * main game loop. This function registers the following event handlers:
 * - Keyboard input handler for key press/release events
 * - Window close handler for clean shutdown
 * - Loop hook that calls game_update every frame
 * 
 * Once all hooks are registered, it starts the MLX42 main loop which will
 * run continuously until the window is closed or the game exits.
 * 
 * @param game Pointer to the initialized game structure
 */
void	game_loop(t_game *game)
{
	mlx_key_hook(game->mlx, handle_keyboard, game);
	mlx_close_hook(game->mlx, on_close, game);
	mlx_loop_hook(game->mlx, game_update, game);
	mlx_loop(game->mlx);
}
