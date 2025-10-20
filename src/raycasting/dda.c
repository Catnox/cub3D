/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Performs one step of the DDA algorithm to traverse the grid.
 * Moves the ray to the next grid intersection based on which side has the shorter distance.
 * Updates ray position and determines which side of the grid cell was hit. */
static void	dda_step(t_ray *ray)
{
	if (ray->side_dist.x < ray->side_dist.y)
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

/* Checks if the ray has hit a wall or gone out of bounds.
 * Returns 1 if a wall is hit or boundaries are exceeded, 0 otherwise.
 * Sets the hit flag when a collision is detected. */
static int	check_wall_hit(t_ray *ray, t_game *game)
{
	if (ray->map_x < 0 || ray->map_x >= game->map->width ||
		ray->map_y < 0 || ray->map_y >= game->map->height)
	{
		ray->hit = 1;
		return (1);
	}
	if (game->map->grid[ray->map_y][ray->map_x] == '1')
	{
		ray->hit = 1;
		return (1);
	}
	return (0);
}

/* Performs the Digital Differential Analyzer (DDA) algorithm for raycasting.
 * Steps through the grid until a wall is hit or maximum steps are reached.
 * Calculates the perpendicular wall distance for proper rendering. */
void	perform_dda(t_ray *ray, t_game *game)
{
	int	max_steps;
	
	max_steps = 0;
	while (ray->hit == 0 && max_steps < game->map->width + game->map->height)
	{
		max_steps++;
		dda_step(ray);
		if (check_wall_hit(ray, game))
			break;
	}
	
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player->pos.x + (1 - ray->step_x) / 2) / ray->dir.x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player->pos.y + (1 - ray->step_y) / 2) / ray->dir.y;
	if (ray->perp_wall_dist <= 0)
		ray->perp_wall_dist = 0.1;
}
