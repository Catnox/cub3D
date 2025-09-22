/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/* BONUS FEATURE DISABLED - door coordinates struct not needed for non-bonus */
/*
typedef struct s_door_coords
{
	double	ray_x;
	double	ray_y;
	double	ray_dir_x;
	double	ray_dir_y;
	double	check_x;
	double	check_y;
	int		map_x;
	int		map_y;
	int		found;
}	t_door_coords;
*/

/**
 * Handle special key interactions (doors and quick actions)
 * 
 * Processes space bar for door interaction and R key for reload/quick turn.
 * Uses raycasting first, then adjacent check for doors.
 * 
 * @param game Pointer to game structure
 * @param key The key that was pressed
 */
static void	handle_special_keys(t_game *game, int key)
{
	/* BONUS FEATURE DISABLED - door interaction */
	// if (key == MLX_KEY_SPACE)
	// {
	//     // Door interaction logic removed for non-bonus version
	//     // Space bar does nothing in non-bonus version
	// }
	
	if (key == MLX_KEY_R)
	{
		/* BONUS FEATURE DISABLED - weapon reload */
		// if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT) || 
		//     mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
		//     reload_weapon(game);
		// else
		rotate_player(game, 3.14159);  // Just do quick turn for non-bonus
	}
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
	/* BONUS FEATURE DISABLED FOR NON-BONUS VERSION */
	// if (game->game_over)
	// {
	//     handle_game_over_input(game, keydata);
	//     return;
	// }
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(game->mlx);
		return ;
	}
	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_R)
			handle_special_keys(game, keydata.key);
		/* BONUS FEATURE DISABLED - shooting */
		// else if (keydata.key == MLX_KEY_ENTER)
		//     enhanced_shoot_weapon(game);
	}
}

/**
 * Handle continuous movement and rotation input
 * 
 * Processes keys that can be held down for continuous movement/rotation.
 * Includes WASD movement, arrow key movement/rotation, and Q/E fast rotation.
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
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		move_player(game, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		move_player(game, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game, -ROT_SPEED);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game, ROT_SPEED);
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		rotate_player(game, -ROT_SPEED * 2);
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		rotate_player(game, ROT_SPEED * 2);
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
	
	/* BONUS FEATURES DISABLED FOR NON-BONUS VERSION */
	// if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL) || 
	//     mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
	// {
	//     if (!game->player->weapon.is_shooting)
	//         enhanced_shoot_weapon(game);
	// }
}
