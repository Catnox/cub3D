/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/27 11:16:56 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * Callback function for window resize events
 * 
 * This function is called whenever the window is resized.
 * It resizes the main image to match the new window dimensions.
 * 
 * @param width New window width
 * @param height New window height
 * @param param Pointer to game structure
 */
static void	resize_callback(int32_t width, int32_t height, void *param)
{
	t_game		*game;
	mlx_image_t	*new_img;

	game = (t_game *)param;
	if (!game || !game->mlx)
		return ;
	if (width <= 0 || height <= 0)
		return ;
	mlx_delete_image(game->mlx, game->img);
	new_img = mlx_new_image(game->mlx, width, height);
	if (!new_img)
		return ;
	game->img = new_img;
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
	{
		mlx_delete_image(game->mlx, game->img);
		return ;
	}
	render_frame(game);
}

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
	game->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D", true);
	if (!game->mlx)
		return (1);
	game->img = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img)
		return (1);
	return (0);
}

/**
 * Master initialization function for the entire game (NON-BONUS VERSION)
 * 
 * This function initializes all necessary components for the game to run.
 * It performs the following steps in order:
 * 1. Parses the .cub map file for textures, colors, and map data
 * 2. Locates the player's starting position on the map
 * 3. Initializes MLX42 graphics system and game state
 * 4. Sets up basic player data (position and direction only)
 * 5. Displays the main image buffer on the window
 * 6. Sets up window resize callback
 *
 * @param game Pointer to the main game structure to initialize
 * @param map_file Path to the .cub map file to load
 * @return 0 on successful initialization, 1 on any failure
 */
int	init_game(t_game *game, char *map_file)
{
	if (parse_cub_file(game, map_file) != 0)
		return (error_exit_init(game, "Failed to parse .cub file"));
	if (find_player_position(game) != 0)
		return (error_exit_init(game, "Failed to find player position"));
	if (init_game_state_n_mlx(game) != 0)
		return (error_exit_init(game,
				"Failed to initialize game state and MLX42"));
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_exit_init(game, "Failed to display image"));
	mlx_resize_hook(game->mlx, resize_callback, game);
	game->is_running = 1;
	return (0);
}
