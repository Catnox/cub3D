/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

/**
 * Set player direction and camera plane based on spawn character
 * 
 * Uses a lookup table approach to set the direction vector and
 * camera plane for raycasting based on the player's spawn orientation.
 * 
 * @param player Pointer to player structure
 * @param dir Direction character (N, S, E, W)
 */
static void	set_player_direction(t_player *player, char dir)
{
	//fix
	const t_direction_data	directions[4] = {
		{0.0, -1.0, 0.66, 0.0},
		{0.0, 1.0, -0.66, 0.0},
		{1.0, 0.0, 0.0, 0.66},
		{-1.0, 0.0, 0.0, -0.66}
	};
	const char				dir_chars[4] = {'N', 'S', 'E', 'W'};
	int						i;

	i = 0;
	while (i < 4)
	{
		if (dir_chars[i] == dir)
		{
			player->dir.x = directions[i].dir_x;
			player->dir.y = directions[i].dir_y;
			player->plane.x = directions[i].plane_x;
			player->plane.y = directions[i].plane_y;
			return;
		}
		i++;
	}
}

/**
 * Process a found player character at given coordinates
 * 
 * Sets up the player position, direction, and clears the map cell.
 * 
 * @param game Pointer to game structure
 * @param x X coordinate of player
 * @param y Y coordinate of player
 * @param spawn_dir Direction character found
 */
static void	setup_player_at_position(t_game *game, int x, int y, char spawn_dir)
{
	game->player->pos.x = x + 0.5;
	game->player->pos.y = y + 0.5;
	set_player_direction(game->player, spawn_dir);
	game->map->grid[y][x] = '0';
	printf("Player spawn at (%.1f, %.1f) facing %c\n", 
		game->player->pos.x, game->player->pos.y, spawn_dir);
}

/**
 * Scan map for player starting position
 * 
 * Searches the entire map grid for player spawn characters (N,S,E,W).
 * Ensures exactly one player position exists.
 * 
 * @param game Pointer to game structure
 * @return 0 on success, 1 if no player or multiple players found
 */
int	find_player_position(t_game *game)
{
	int	x;
	int	player_found;
	int	y;

	player_found = 0;
	y = -1;
	while (++y < game->map->height)
	{
		x = -1;
		while (++x < game->map->width)
		{
			if (is_player_char(game->map->grid[y][x]))
			{
				if (player_found)
					return (ft_putendl_fd("Error: Multiple player positions found", 2), 1);
				setup_player_at_position(game, x, y, game->map->grid[y][x]);
				player_found = 1;
			}
		}
	}
	if (!player_found)
		return (ft_putendl_fd("Error: No player position found in map", 2), 1);
	return (0);
}
