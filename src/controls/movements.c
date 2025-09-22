/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

/**
 * Check basic collision at given coordinates
 * 
 * Performs collision detection at a specific point, checking for:
 * - Map boundaries (out of bounds)
 * - Walls (grid value '1')
 * - Closed doors (grid value 'D' with open_state < 0.8)
 * 
 * @param game Pointer to game structure containing map data
 * @param x X coordinate to check for collision
 * @param y Y coordinate to check for collision
 * @return 1 if collision detected, 0 if position is free
 */
static int	check_collision(t_game *game, double x, double y)
{
	int	map_x, map_y;
	
	if (x < 0 || y < 0)
		return (1);
	map_x = (int)x;
	map_y = (int)y;
	if (map_x >= game->map->width || map_y >= game->map->height)
		return (1);
	if (game->map->grid[map_y][map_x] == '1')
		return (1);
	/* BONUS FEATURE DISABLED - doors */
	// if (game->map->grid[map_y][map_x] == 'D')
	// {
	//     t_door *door = find_door(game, map_x, map_y);
	//     if (door && door->open_state < 0.8)
	//         return (1);
	// }
	return (0);
}

/**
 * Check collision with player collision buffer
 * 
 * Enhanced collision detection that checks multiple points around the player
 * to create a collision buffer/box. This prevents the player from getting
 * too close to walls and provides smoother movement experience.
 * 
 * Checks 5 points: center and 4 corners of collision box
 * 
 * @param game Pointer to game structure containing map data
 * @param x Center X coordinate of player position
 * @param y Center Y coordinate of player position
 * @return 1 if any collision detected, 0 if all positions are free
 */
static int	check_collision_with_buffer(t_game *game, double x, double y)
{
	const double buffer = 0.15; // Collision buffer distance from walls
	
	if (check_collision(game, x - buffer, y - buffer) ||
		check_collision(game, x + buffer, y - buffer) ||
		check_collision(game, x - buffer, y + buffer) ||
		check_collision(game, x + buffer, y + buffer))
		return (1);
	if (check_collision(game, x, y))
		return (1);
	
	return (0);
}

/**
 * Calculate new position based on movement direction
 * 
 * Computes the target position for player movement based on direction code.
 * Uses current player direction and perpendicular vectors for strafing.
 * 
 * @param game Pointer to game structure containing player data
 * @param direction Movement direction code (1=forward, -1=back, 2=left, 3=right)
 * @return New target position as t_vec2
 */
static t_vec2	calculate_new_position(t_game *game, int direction)
{
	t_vec2	new_pos;

	new_pos = game->player->pos;
	if (direction == 1)
	{
		new_pos.x += game->player->dir.x * MOVE_SPEED;
		new_pos.y += game->player->dir.y * MOVE_SPEED;
	}
	else if (direction == -1)
	{
		new_pos.x -= game->player->dir.x * MOVE_SPEED;
		new_pos.y -= game->player->dir.y * MOVE_SPEED;
	}
	else if (direction == 2)
	{
		new_pos.x += game->player->dir.y * MOVE_SPEED;
		new_pos.y -= game->player->dir.x * MOVE_SPEED;
	}
	else if (direction == 3)
	{
		new_pos.x -= game->player->dir.y * MOVE_SPEED;
		new_pos.y += game->player->dir.x * MOVE_SPEED;
	}
	return (new_pos);
}

/**
 * Apply movement with wall sliding collision detection
 * 
 * Tests X and Y movement separately to enable wall sliding.
 * If diagonal movement hits a wall, player can still slide along it.
 * 
 * @param game Pointer to game structure containing player data
 * @param new_pos Target position to move to
 */
static void	apply_movement_with_sliding(t_game *game, t_vec2 new_pos)
{
	t_vec2	test_pos;

	// Try X movement first
	test_pos = game->player->pos;
	test_pos.x = new_pos.x;
	if (!check_collision_with_buffer(game, test_pos.x, test_pos.y))
		game->player->pos.x = test_pos.x;

	// Then try Y movement
	test_pos = game->player->pos;
	test_pos.y = new_pos.y;
	if (!check_collision_with_buffer(game, test_pos.x, test_pos.y))
		game->player->pos.y = test_pos.y;
}

/**
 * Move player in specified direction with collision detection
 * 
 * Main movement function that coordinates position calculation and
 * collision-aware movement application with wall sliding support.
 * 
 * @param game Pointer to game structure containing player data
 * @param direction Integer code specifying movement direction
 */
void	move_player(t_game *game, int direction)
{
	t_vec2	new_pos;

	new_pos = calculate_new_position(game, direction);
	apply_movement_with_sliding(game, new_pos);
}

/**
 * Rotate player view by specified angle
 * 
 * Performs 2D rotation transformation on both the player's direction vector
 * and camera plane vector. Uses standard rotation matrix mathematics:
 * - new_x = x * cos(θ) - y * sin(θ)
 * - new_y = x * sin(θ) + y * cos(θ)
 * 
 * Both direction and plane vectors must be rotated to maintain proper
 * field of view and perspective in the raycasting engine.
 * 
 * @param game Pointer to game structure containing player data
 * @param angle Rotation angle in radians (positive = clockwise)
 */
void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;
	
	old_dir_x = game->player->dir.x;
	game->player->dir.x = game->player->dir.x * cos(angle) - game->player->dir.y * sin(angle);
	game->player->dir.y = old_dir_x * sin(angle) + game->player->dir.y * cos(angle);
	
	old_plane_x = game->player->plane.x;
	game->player->plane.x = game->player->plane.x * cos(angle) - game->player->plane.y * sin(angle);
	game->player->plane.y = old_plane_x * sin(angle) + game->player->plane.y * cos(angle);
}
