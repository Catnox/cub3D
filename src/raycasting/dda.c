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

#include "../../includes/cub3d.h"

void	perform_dda(t_ray *ray, t_game *game)
{
	int	max_steps;
	
	max_steps = 0;
	// Perform DDA
	while (ray->hit == 0 && max_steps < game->map->width + game->map->height)
	{
		max_steps++;
		
		// Jump to next map square, either in x-direction, or in y-direction
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
		
		// Check bounds first to avoid segfault
		if (ray->map_x < 0 || ray->map_x >= game->map->width ||
			ray->map_y < 0 || ray->map_y >= game->map->height)
		{
			ray->hit = 1; // Hit boundary, treat as wall
			break;
		}
		
		// Check if ray hit a wall
		if (game->map->grid[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
	
	// Calculate distance correctly using player position
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player->pos.x + (1 - ray->step_x) / 2) / ray->dir.x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player->pos.y + (1 - ray->step_y) / 2) / ray->dir.y;
		
	// Avoid division by zero or negative distances
	if (ray->perp_wall_dist <= 0)
		ray->perp_wall_dist = 0.1;
}
