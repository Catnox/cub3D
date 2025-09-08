/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shooting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	check_wall_hit(t_game *game, int map_x, int map_y)
{
	if (map_x < 0 || map_x >= game->map->width || 
		map_y < 0 || map_y >= game->map->height)
		return (1);  // Out of bounds = wall
	
	char cell = game->map->grid[map_y][map_x];
	return (cell == '1' || cell == 'D');  // Wall or door stops the shot
}

static void	process_hit(t_game *game, double hit_x, double hit_y, double distance)
{
	int	map_x = (int)hit_x;
	int	map_y = (int)hit_y;
	
	// Check for enemy hits first (in the exact hit position)
	for (int i = 0; i < game->enemies->count; i++)
	{
		t_enemy *enemy = &game->enemies->enemies[i];
		if (!enemy->is_alive)
			continue;
		
		double dx = enemy->pos.x - hit_x;
		double dy = enemy->pos.y - hit_y;
		double enemy_distance = sqrt(dx * dx + dy * dy);
		
		// If shot passes very close to enemy (within their "hitbox")
		if (enemy_distance < ENEMY_SIZE / 2)
		{
			damage_enemy(game, i, game->player->weapon.damage);
			printf("Direct hit on enemy %d at distance %.2f!\n", i, distance);
			return;
		}
	}
	
	// Check what was hit on the map
	if (map_x >= 0 && map_x < game->map->width && 
		map_y >= 0 && map_y < game->map->height)
	{
		char cell = game->map->grid[map_y][map_x];
		
		if (cell == '1')
		{
			printf("Hit wall at (%.2f, %.2f) distance: %.2f\n", hit_x, hit_y, distance);
		}
		else if (cell == 'D')
		{
			printf("Hit door at (%.2f, %.2f) distance: %.2f\n", hit_x, hit_y, distance);
			// Could damage/destroy door here
		}
		else
		{
			printf("Shot missed - hit empty space at (%.2f, %.2f) distance: %.2f\n", 
				hit_x, hit_y, distance);
		}
	}
}

void	cast_shot_ray(t_game *game)
{
	double	ray_x = game->player->pos.x;
	double	ray_y = game->player->pos.y;
	double	ray_dir_x = game->player->dir.x;
	double	ray_dir_y = game->player->dir.y;
	double	step_size = 0.01;  // Small steps for precise hit detection
	double	max_distance = 20.0;  // Maximum shot range
	double	distance;
	
	// Cast ray until we hit something or reach max distance
	for (distance = step_size; distance <= max_distance; distance += step_size)
	{
		double check_x = ray_x + ray_dir_x * distance;
		double check_y = ray_y + ray_dir_y * distance;
		
		// Check for enemy hits along the ray path
		for (int i = 0; i < game->enemies->count; i++)
		{
			t_enemy *enemy = &game->enemies->enemies[i];
			if (!enemy->is_alive)
				continue;
			
			double dx = enemy->pos.x - check_x;
			double dy = enemy->pos.y - check_y;
			double enemy_distance = sqrt(dx * dx + dy * dy);
			
			// If ray passes through enemy hitbox
			if (enemy_distance < ENEMY_SIZE / 2)
			{
				damage_enemy(game, i, game->player->weapon.damage);
				printf("Shot hit enemy %d at distance %.2f!\n", i, distance);
				return; // Shot stops at first enemy hit
			}
		}
		
		int map_x = (int)check_x;
		int map_y = (int)check_y;
		
		// Check if we hit a wall or door
		if (check_wall_hit(game, map_x, map_y))
		{
			process_hit(game, check_x, check_y, distance);
			return;
		}
	}
	
	// Shot didn't hit anything within range
	printf("Shot traveled max distance (%.1f) without hitting anything\n", max_distance);
}

void	enhanced_shoot_weapon(t_game *game)
{
	// Check if weapon is ready to shoot and player has ammo
	if (game->player->weapon.is_shooting || game->player->ammo < game->player->weapon.ammo_per_shot)
		return;

	// Start shooting animation
	game->player->weapon.is_shooting = 1;
	game->player->weapon.current_frame = 1;  // Start with first shooting frame
	game->player->weapon.animation_timer = 0.0;

	// Consume ammo
	game->player->ammo -= game->player->weapon.ammo_per_shot;

	// Cast a ray to see what we hit
	cast_shot_ray(game);
	
	printf("BANG! Shot fired. Ammo remaining: %d\n", game->player->ammo);
}
