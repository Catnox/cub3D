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

void	handle_keyboard(mlx_key_data_t keydata, void *param)
{
	t_game	*game;
	
	game = (t_game *)param;
	
	// Handle game over input first
	if (game->game_over)
	{
		handle_game_over_input(game, keydata);
		return;
	}
	
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(game->mlx);
		return ;
	}
	
	// Action keys (single press)
	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_SPACE)
		{
			// Improved door detection using raycasting technique
			// Cast a short ray in front of the player to find doors
			int door_found = 0;
			double ray_x = game->player->pos.x;
			double ray_y = game->player->pos.y;
			double ray_dir_x = game->player->dir.x;
			double ray_dir_y = game->player->dir.y;
			
			// Step along the ray to find doors (max distance 2.0)
			for (double step = 0.1; step <= 2.0 && !door_found; step += 0.1)
			{
				double check_x = ray_x + ray_dir_x * step;
				double check_y = ray_y + ray_dir_y * step;
				
				int map_x = (int)check_x;
				int map_y = (int)check_y;
				
				// Check bounds
				if (map_x < 0 || map_x >= game->map->width || 
					map_y < 0 || map_y >= game->map->height)
					break;
				
				// Stop if we hit a wall
				if (game->map->grid[map_y][map_x] == '1')
					break;
				
				// Check if we found a door
				if (game->map->grid[map_y][map_x] == 'D')
				{
					toggle_door(game, map_x, map_y);
					printf("Door toggled at (%d, %d) [raycast distance: %.2f]\n", 
						map_x, map_y, step);
					door_found = 1;
				}
			}
			
			// If no door found with raycast, check immediate vicinity
			if (!door_found)
			{
				int player_x = (int)game->player->pos.x;
				int player_y = (int)game->player->pos.y;
				
				// Check adjacent cells
				for (int dy = -1; dy <= 1 && !door_found; dy++)
				{
					for (int dx = -1; dx <= 1 && !door_found; dx++)
					{
						if (dx == 0 && dy == 0)
							continue;
							
						int check_x = player_x + dx;
						int check_y = player_y + dy;
						
						if (check_x >= 0 && check_x < game->map->width && 
							check_y >= 0 && check_y < game->map->height &&
							game->map->grid[check_y][check_x] == 'D')
						{
							// Only activate if very close (better collision detection)
							double door_dist = sqrt((check_x + 0.5 - game->player->pos.x) * 
													(check_x + 0.5 - game->player->pos.x) +
													(check_y + 0.5 - game->player->pos.y) * 
													(check_y + 0.5 - game->player->pos.y));
							
							if (door_dist <= 1.2) // Close enough
							{
								toggle_door(game, check_x, check_y);
								printf("Door toggled at (%d, %d) [adjacent distance: %.2f]\n", 
									check_x, check_y, door_dist);
								door_found = 1;
							}
						}
					}
				}
			}
		}
		else if (keydata.key == MLX_KEY_R)
		{
			if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_SHIFT) || 
				mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_SHIFT))
			{
				// Shift+R: Reload weapon
				reload_weapon(game);
			}
			else
			{
				// R: Fast 180 degree turn
				rotate_player(game, 3.14159); // π radians = 180 degrees
				printf("Quick turn!\n");
			}
		}
	}
	
	// Shooting controls
	if (keydata.key == MLX_KEY_ENTER && keydata.action == MLX_PRESS)
	{
		enhanced_shoot_weapon(game);
	}
	// Alternative shooting with mouse buttons handled in mouse.c
}

void	handle_continuous_input(t_game *game)
{
	// Movement keys - can be combined for diagonal movement
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move_player(game, 1);      // Forward
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move_player(game, -1);     // Backward
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move_player(game, 2);      // Strafe left
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move_player(game, 3);      // Strafe right
	
	// Arrow keys for movement too
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		move_player(game, 1);      // Forward
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		move_player(game, -1);     // Backward
	
	// Rotation keys
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate_player(game, -ROT_SPEED);  // Rotate left
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate_player(game, ROT_SPEED);   // Rotate right
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		rotate_player(game, -ROT_SPEED * 2);  // Fast rotate left
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		rotate_player(game, ROT_SPEED * 2);   // Fast rotate right
	
	// Continuous shooting (Wolfenstein style)
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT_CONTROL) || 
		mlx_is_key_down(game->mlx, MLX_KEY_RIGHT_CONTROL))
	{
		// Only shoot if weapon is ready (not currently shooting)
		if (!game->player->weapon.is_shooting)
			enhanced_shoot_weapon(game);
	}
}
