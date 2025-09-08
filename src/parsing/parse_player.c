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

#include "../../includes/cub3d.h"

static int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static void	set_player_direction(t_player *player, char dir)
{
	if (dir == 'N')
	{
		player->dir.x = 0.0;
		player->dir.y = -1.0;
		player->plane.x = 0.66;
		player->plane.y = 0.0;
	}
	else if (dir == 'S')
	{
		player->dir.x = 0.0;
		player->dir.y = 1.0;
		player->plane.x = -0.66;
		player->plane.y = 0.0;
	}
	else if (dir == 'E')
	{
		player->dir.x = 1.0;
		player->dir.y = 0.0;
		player->plane.x = 0.0;
		player->plane.y = 0.66;
	}
	else if (dir == 'W')
	{
		player->dir.x = -1.0;
		player->dir.y = 0.0;
		player->plane.x = 0.0;
		player->plane.y = -0.66;
	}
}

int	find_player_position(t_game *game)
{
	int	x, y;
	int	player_found;
	
	player_found = 0;
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			if (is_player_char(game->map->grid[y][x]))
			{
				if (player_found)
				{
					ft_putendl_fd("Error: Multiple player positions found", 2);
					return (1);
				}
				
				// Set player position (center of the cell)
				game->player->pos.x = x + 0.5;
				game->player->pos.y = y + 0.5;
				
				// Set player direction based on character
				char spawn_dir = game->map->grid[y][x];
				set_player_direction(game->player, spawn_dir);
				
				// Replace player character with empty space
				game->map->grid[y][x] = '0';
				
				player_found = 1;
				printf("Player spawn at (%.1f, %.1f) facing %c\n", 
					game->player->pos.x, game->player->pos.y, spawn_dir);
			}
			x++;
		}
		y++;
	}
	
	if (!player_found)
	{
		ft_putendl_fd("Error: No player position found in map", 2);
		return (1);
	}
	
	return (0);
}
