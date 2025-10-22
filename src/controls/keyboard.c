/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/22 23:20:29 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	on_close(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	cleanup_game(game);
	exit(1);
}

/**
 * Handle discrete keyboard input events
 * 
 * Processes single key press events including escape, special keys,
 * and shooting. Delegates game over handling to specialized function.
 * 
 * @param keydata Key event data structure
 * @param param Game structure pointer (void* for MLX42 compatibility)
 */
void	handle_keyboard(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		cleanup_game(game);
		exit(0);
	}
}

/**
 * Handle continuous movement and rotation input
 * 
 * Processes keys that can be held down for continuous movement/rotation.
 * Includes WASD movement, arrow key rotation, and Q/E fast rotation.
 * 
 * @param game Pointer to game structure
 */
static void	handle_movement_rotation(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player(game, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player(game, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player(game, 2);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player(game, 3);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game, -ROT_SPEED);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game, ROT_SPEED);
}

/**
 * Handle all continuous input from keyboard and combat
 * 
 * Main coordinator function that processes all continuous input:
 * movement, rotation, and combat. Combines all input handling for efficiency.
 * 
 * @param game Pointer to game structure
 */
void	handle_continuous_input(t_game *game)
{
	handle_movement_rotation(game);
}
