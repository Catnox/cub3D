/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:30:00 by mknoll            #+#    #+#             */
/*   Updated: 2025/10/20 11:04:03 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
int	check_collision(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	if (x < 0 || y < 0)
		return (1);
	map_x = (int)x;
	map_y = (int)y;
	if (map_x >= game->map->width || map_y >= game->map->height)
		return (1);
	if (game->map->grid[map_y][map_x] == '1')
		return (1);
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
int	check_collision_with_buffer(t_game *game, double x, double y)
{
	const double	buffer = 0.15;

	if (check_collision(game, x, y))
		return (1);
	if (check_collision(game, x + buffer, y + buffer))
		return (1);
	if (check_collision(game, x - buffer, y + buffer))
		return (1);
	if (check_collision(game, x + buffer, y - buffer))
		return (1);
	if (check_collision(game, x - buffer, y - buffer))
		return (1);
	return (0);
}

t_vec2	calculate_new_position(t_game *game, int direction)
{
	t_vec2			new_pos;
	const double	speed = 0.05;

	new_pos = game->player->pos;
	if (direction == 1)
	{
		new_pos.x += game->player->dir.x * speed;
		new_pos.y += game->player->dir.y * speed;
	}
	else if (direction == -1)
	{
		new_pos.x -= game->player->dir.x * speed;
		new_pos.y -= game->player->dir.y * speed;
	}
	else if (direction == 2)
	{
		new_pos.x -= game->player->plane.x * speed;
		new_pos.y -= game->player->plane.y * speed;
	}
	else if (direction == 3)
	{
		new_pos.x += game->player->plane.x * speed;
		new_pos.y += game->player->plane.y * speed;
	}
	return (new_pos);
}

void	move_player(t_game *game, int direction)
{
	t_vec2	new_pos;

	new_pos = calculate_new_position(game, direction);
	if (!check_collision_with_buffer(game, new_pos.x, game->player->pos.y))
		game->player->pos.x = new_pos.x;
	if (!check_collision_with_buffer(game, game->player->pos.x, new_pos.y))
		game->player->pos.y = new_pos.y;
}
