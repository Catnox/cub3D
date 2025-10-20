/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/10/20 10:12:59 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Calculates the ray direction based on camera position and screen x coordinate
 * Converts screen pixel position to world ray direction using camera plane. */
static void	calculate_ray_direction(t_ray *ray, t_player *player, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->dir.x = player->dir.x + player->plane.x * camera_x;
	ray->dir.y = player->dir.y + player->plane.y * camera_x;
}

/* Calculates delta distances for DDA algorithm.
 * Delta distance represents the distance the ray
 * travels for one unit in x or y direction. */
static void	calculate_delta_distances(t_ray *ray)
{
	if (ray->dir.x == 0)
		ray->delta_dist.x = 1e30;
	else
		ray->delta_dist.x = fabs(1 / ray->dir.x);
	if (ray->dir.y == 0)
		ray->delta_dist.y = 1e30;
	else
		ray->delta_dist.y = fabs(1 / ray->dir.y);
}

/* Calculates step directions and initial side distances for DDA.
 * Determines which direction to step in the grid and the distance
 * to the next grid line. */
static void	calculate_step_and_side_dist(t_ray *ray, t_player *player)
{
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist.x = (player->pos.x - ray->map_x) * ray->delta_dist.x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist.x = (ray->map_x + 1.0 - player->pos.x)
			* ray->delta_dist.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist.y = (player->pos.y - ray->map_y) * ray->delta_dist.y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist.y = (ray->map_y + 1.0 - player->pos.y)
			* ray->delta_dist.y;
	}
}

/* Initializes a ray for raycasting at a given screen x coordinate.
 * Sets up all necessary parameters for the DDA algorithm including direction,
 * position, delta distances, and step values. */
void	init_ray(t_ray *ray, t_player *player, int x)
{
	calculate_ray_direction(ray, player, x);
	ray->map_x = (int)player->pos.x;
	ray->map_y = (int)player->pos.y;
	calculate_delta_distances(ray);
	calculate_step_and_side_dist(ray, player);
	ray->hit = 0;
}

/* Main raycasting function that renders the entire screen.
 * Casts one ray for each screen column, performs DDA, 
 and draws the resulting walls. */
void	cast_rays(t_game *game)
{
	t_ray	ray;
	int		x;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		init_ray(&ray, game->player, x);
		perform_dda(&ray, game);
		draw_walls(game, x, &ray);
		x++;
	}
}
