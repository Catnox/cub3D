/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	init_doors(t_game *game)
{
	int	x, y, door_count;
	
	// First pass: count doors
	door_count = 0;
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			if (game->map->grid[y][x] == 'D')
				door_count++;
			x++;
		}
		y++;
	}
	
	// Allocate memory for doors
	game->map->doors = ft_calloc(door_count, sizeof(t_door));
	if (!game->map->doors)
		return;
	
	game->map->door_count = door_count;
	
	// Second pass: initialize doors
	door_count = 0;
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			if (game->map->grid[y][x] == 'D')
			{
				game->map->doors[door_count].x = x;
				game->map->doors[door_count].y = y;
				game->map->doors[door_count].open_state = 0.0;
				game->map->doors[door_count].is_opening = 0;
				game->map->doors[door_count].open_speed = 0.05;
				door_count++;
			}
			x++;
		}
		y++;
	}
}

void	update_doors(t_game *game)
{
	int	i;
	
	i = 0;
	while (i < game->map->door_count)
	{
		if (game->map->doors[i].is_opening == 1) // Opening
		{
			game->map->doors[i].open_state += game->map->doors[i].open_speed;
			if (game->map->doors[i].open_state >= 1.0)
			{
				game->map->doors[i].open_state = 1.0;
				game->map->doors[i].is_opening = 0;
			}
		}
		else if (game->map->doors[i].is_opening == -1) // Closing
		{
			game->map->doors[i].open_state -= game->map->doors[i].open_speed;
			if (game->map->doors[i].open_state <= 0.0)
			{
				game->map->doors[i].open_state = 0.0;
				game->map->doors[i].is_opening = 0;
			}
		}
		i++;
	}
}

void	toggle_door(t_game *game, int x, int y)
{
	t_door	*door;
	
	door = find_door(game, x, y);
	if (!door)
		return;
	
	if (door->open_state == 0.0) // Closed, open it
		door->is_opening = 1;
	else if (door->open_state == 1.0) // Open, close it
		door->is_opening = -1;
	else // In motion, reverse direction
		door->is_opening = -door->is_opening;
}

t_door	*find_door(t_game *game, int x, int y)
{
	int	i;
	
	i = 0;
	while (i < game->map->door_count)
	{
		if (game->map->doors[i].x == x && game->map->doors[i].y == y)
			return (&game->map->doors[i]);
		i++;
	}
	return (NULL);
}

int	is_door_position(t_game *game, int x, int y)
{
	return (find_door(game, x, y) != NULL);
}
