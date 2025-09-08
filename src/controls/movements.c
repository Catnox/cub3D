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

static int	check_collision(t_game *game, double x, double y)
{
	int	map_x, map_y;
	
	// Check bounds first
	if (x < 0 || y < 0)
		return (1);
	
	map_x = (int)x;
	map_y = (int)y;
	
	if (map_x >= game->map->width || map_y >= game->map->height)
		return (1);
	
	// Check for wall
	if (game->map->grid[map_y][map_x] == '1')
		return (1);
	
	// Check for closed door
	if (game->map->grid[map_y][map_x] == 'D')
	{
		t_door *door = find_door(game, map_x, map_y);
		if (door && door->open_state < 0.8)
			return (1); // Door is closed
	}
	
	return (0); // No collision
}

static int	check_collision_with_buffer(t_game *game, double x, double y)
{
	const double buffer = 0.15; // Collision buffer distance from walls
	
	// Check the four corners of the player's collision box
	if (check_collision(game, x - buffer, y - buffer) ||
		check_collision(game, x + buffer, y - buffer) ||
		check_collision(game, x - buffer, y + buffer) ||
		check_collision(game, x + buffer, y + buffer))
		return (1);
	
	// Also check the center
	if (check_collision(game, x, y))
		return (1);
	
	return (0);
}

void	move_player(t_game *game, int direction)
{
	t_vec2	new_pos;
	
	new_pos = game->player->pos;
	
	if (direction == 1) // Forward
	{
		new_pos.x += game->player->dir.x * MOVE_SPEED;
		new_pos.y += game->player->dir.y * MOVE_SPEED;
	}
	else if (direction == -1) // Backward
	{
		new_pos.x -= game->player->dir.x * MOVE_SPEED;
		new_pos.y -= game->player->dir.y * MOVE_SPEED;
	}
	else if (direction == 2) // Strafe left
	{
		new_pos.x += game->player->dir.y * MOVE_SPEED;
		new_pos.y -= game->player->dir.x * MOVE_SPEED;
	}
	else if (direction == 3) // Strafe right
	{
		new_pos.x -= game->player->dir.y * MOVE_SPEED;
		new_pos.y += game->player->dir.x * MOVE_SPEED;
	}
	
	// Advanced collision detection with buffer
	// Try X movement first (sliding along walls)
	t_vec2 test_pos = game->player->pos;
	test_pos.x = new_pos.x;
	if (!check_collision_with_buffer(game, test_pos.x, test_pos.y))
		game->player->pos.x = test_pos.x;
	
	// Then try Y movement 
	test_pos = game->player->pos;
	test_pos.y = new_pos.y;
	if (!check_collision_with_buffer(game, test_pos.x, test_pos.y))
		game->player->pos.y = test_pos.y;
}

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
