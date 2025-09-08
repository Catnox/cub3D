/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	init_enemies(t_game *game)
{
	game->enemies = malloc(sizeof(t_enemies));
	if (!game->enemies)
		return (0);
	
	ft_memset(game->enemies, 0, sizeof(t_enemies));
	game->enemies->count = 0;
	game->enemies->spawn_timer = 0.0;
	
	// For now, we'll create simple colored textures for enemies
	// In a full implementation, you'd load actual enemy sprites
	game->enemies->texture_guard = NULL;
	game->enemies->texture_officer = NULL;
	game->enemies->texture_dog = NULL;
	
	// Spawn some initial enemies on the map
	// Look for 'G' characters in the map for enemy positions (G = Guard)
	for (int y = 0; y < game->map->height; y++)
	{
		for (int x = 0; x < game->map->width; x++)
		{
			if (game->map->grid[y][x] == 'G')
			{
				spawn_enemy(game, x + 0.5, y + 0.5, 0); // Guard type
				game->map->grid[y][x] = '0'; // Replace with empty space
			}
		}
	}
	
	return (1);
}

void	spawn_enemy(t_game *game, double x, double y, int type)
{
	if (game->enemies->count >= MAX_ENEMIES)
		return;
	
	t_enemy *enemy = &game->enemies->enemies[game->enemies->count];
	
	enemy->pos.x = x;
	enemy->pos.y = y;
	enemy->dir.x = 1.0; // Initially facing east
	enemy->dir.y = 0.0;
	enemy->is_alive = 1;
	enemy->is_attacking = 0;
	enemy->attack_timer = 0.0;
	enemy->move_timer = 0.0;
	enemy->enemy_type = type;
	enemy->state = 0; // Patrol state
	enemy->target_pos = enemy->pos;
	
	// Initialize A* pathfinding fields
	enemy->path = NULL;
	enemy->path_length = 0;
	enemy->path_step = 0;
	enemy->pathfind_timer = 0.0;
	
	// Set stats based on enemy type
	switch (type)
	{
		case 0: // Guard
			enemy->max_hp = ENEMY_HEALTH;
			enemy->hp = enemy->max_hp;
			enemy->speed = ENEMY_SPEED;
			enemy->damage = ENEMY_DAMAGE;
			enemy->detection_range = 8.0;
			break;
		case 1: // Officer
			enemy->max_hp = ENEMY_HEALTH * 1.5;
			enemy->hp = enemy->max_hp;
			enemy->speed = ENEMY_SPEED * 0.8;
			enemy->damage = ENEMY_DAMAGE * 1.5;
			enemy->detection_range = 10.0;
			break;
		case 2: // Dog
			enemy->max_hp = ENEMY_HEALTH * 0.5;
			enemy->hp = enemy->max_hp;
			enemy->speed = ENEMY_SPEED * 2.0;
			enemy->damage = ENEMY_DAMAGE * 0.8;
			enemy->detection_range = 12.0;
			break;
	}
	
	game->enemies->count++;
	printf("Enemy spawned at (%.1f, %.1f) - Type: %d\n", x, y, type);
}

int	enemy_can_see_player(t_game *game, t_enemy *enemy)
{
	double dx = game->player->pos.x - enemy->pos.x;
	double dy = game->player->pos.y - enemy->pos.y;
	double distance = sqrt(dx * dx + dy * dy);
	
	// Too far to detect
	if (distance > enemy->detection_range)
		return (0);
	
	// Normalize direction vector
	dx /= distance;
	dy /= distance;
	
	// Cast ray from enemy to player to check for walls
	double step_size = 0.1;
	for (double d = step_size; d < distance; d += step_size)
	{
		double check_x = enemy->pos.x + dx * d;
		double check_y = enemy->pos.y + dy * d;
		
		int map_x = (int)check_x;
		int map_y = (int)check_y;
		
		if (map_x < 0 || map_x >= game->map->width || 
			map_y < 0 || map_y >= game->map->height)
			return (0);
		
		char cell = game->map->grid[map_y][map_x];
		if (cell == '1') // Wall blocks vision
			return (0);
		
		// Closed doors block vision
		if (cell == 'D')
		{
			t_door *door = find_door(game, map_x, map_y);
			if (door && door->open_state < 0.5)
				return (0);
		}
	}
	
	return (1); // Clear line of sight
}

void	enemy_ai_update(t_game *game, t_enemy *enemy)
{
	if (!enemy->is_alive)
		return;
	
	double dx = game->player->pos.x - enemy->pos.x;
	double dy = game->player->pos.y - enemy->pos.y;
	double distance = sqrt(dx * dx + dy * dy);
	
	// Update pathfinding timer
	enemy->pathfind_timer += 0.016; // ~60fps
	
	// State machine avec A*
	switch (enemy->state)
	{
		case 0: // Patrol
			if (enemy_can_see_player(game, enemy))
			{
				enemy->state = 1; // Alert
				printf("Enemy spotted player!\n");
				// Recalcule immédiatement le chemin
				if (enemy->path)
					free_path(enemy->path, enemy->path_length);
				enemy->path = find_path_astar(game, (int)enemy->pos.x, (int)enemy->pos.y, 
					(int)game->player->pos.x, (int)game->player->pos.y, &enemy->path_length);
				enemy->path_step = 0;
				enemy->pathfind_timer = 0.0;
			}
			else
			{
				// Simple patrol: move randomly
				enemy->move_timer += 0.016;
				if (enemy->move_timer > 2.0) // Change direction every 2 seconds
				{
					enemy->dir.x = (rand() % 3 - 1) * 0.5; // -0.5, 0, or 0.5
					enemy->dir.y = (rand() % 3 - 1) * 0.5;
					enemy->move_timer = 0.0;
				}
			}
			break;
			
		case 1: // Alert (using A* pathfinding)
			if (distance < 1.5)
			{
				enemy->state = 2; // Attacking
				enemy->attack_timer = 0.0;
			}
			else
			{
				// Recalcule le chemin toutes les secondes ou si pas de chemin
				if (!enemy->path || enemy->pathfind_timer > 1.0)
				{
					if (enemy->path)
						free_path(enemy->path, enemy->path_length);
					enemy->path = find_path_astar(game, (int)enemy->pos.x, (int)enemy->pos.y, 
						(int)game->player->pos.x, (int)game->player->pos.y, &enemy->path_length);
					enemy->path_step = 0;
					enemy->pathfind_timer = 0.0;
				}
				
				// Suit le chemin A*
				if (enemy->path && enemy->path_step < enemy->path_length - 1)
				{
					t_vec2 next_pos = get_next_position_on_path(enemy->path, enemy->path_length, enemy->path_step);
					
					// Calcule la direction vers la prochaine étape
					double next_dx = next_pos.x - enemy->pos.x;
					double next_dy = next_pos.y - enemy->pos.y;
					double next_distance = sqrt(next_dx * next_dx + next_dy * next_dy);
					
					if (next_distance < 0.3) // Proche de l'étape suivante
					{
						enemy->path_step++; // Passe à l'étape suivante
					}
					else
					{
						// Se dirige vers l'étape suivante
						enemy->dir.x = next_dx / next_distance;
						enemy->dir.y = next_dy / next_distance;
					}
				}
				else if (!enemy->path)
				{
					// Fallback : mouvement direct si A* échoue
					enemy->dir.x = dx / distance;
					enemy->dir.y = dy / distance;
				}
			}
			break;
			
		case 2: // Attacking
			enemy->attack_timer += 0.016;
			if (enemy->attack_timer > 1.0) // Attack every second
			{
				if (distance < 2.0) // Within attack range
					enemy_attack(game, enemy);
				enemy->attack_timer = 0.0;
				
				// If player moved away, go back to alert
				if (distance > 3.0)
					enemy->state = 1;
			}
			break;
	}
	
	// Move enemy if not attacking
	if (enemy->state != 2)
	{
		double new_x = enemy->pos.x + enemy->dir.x * enemy->speed;
		double new_y = enemy->pos.y + enemy->dir.y * enemy->speed;
		
		// Check collision with walls
		int map_x = (int)new_x;
		int map_y = (int)new_y;
		
		if (map_x >= 0 && map_x < game->map->width && 
			map_y >= 0 && map_y < game->map->height &&
			game->map->grid[map_y][map_x] != '1')
		{
			enemy->pos.x = new_x;
			enemy->pos.y = new_y;
		}
	}
}

void	enemy_attack(t_game *game, t_enemy *enemy)
{
	double dx = game->player->pos.x - enemy->pos.x;
	double dy = game->player->pos.y - enemy->pos.y;
	double distance = sqrt(dx * dx + dy * dy);
	
	if (distance < 2.0) // Close enough to damage player
	{
		game->player->hp -= enemy->damage;
		printf("Enemy attacks! Player HP: %d (-%d)\n", 
			game->player->hp, enemy->damage);
	}
}

void	damage_enemy(t_game *game, int enemy_index, int damage)
{
	if (enemy_index < 0 || enemy_index >= game->enemies->count)
		return;
	
	t_enemy *enemy = &game->enemies->enemies[enemy_index];
	if (!enemy->is_alive)
		return;
	
	enemy->hp -= damage;
	printf("Enemy %d takes %d damage! HP: %d\n", enemy_index, damage, enemy->hp);
	
	if (enemy->hp <= 0)
	{
		enemy->is_alive = 0;
		enemy->state = 3; // Dead
		
		// Nettoie le chemin A* de l'ennemi mort
		if (enemy->path)
		{
			free_path(enemy->path, enemy->path_length);
			enemy->path = NULL;
			enemy->path_length = 0;
		}
		
		game->player->score += 100; // Score for killing enemy
		printf("Enemy %d killed! Score: %d\n", enemy_index, game->player->score);
	}
	else
	{
		// Become alert when hit
		enemy->state = 1;
	}
}

void	update_enemies(t_game *game)
{
	for (int i = 0; i < game->enemies->count; i++)
	{
		enemy_ai_update(game, &game->enemies->enemies[i]);
	}
}

void	cleanup_enemies(t_game *game)
{
	int i;

	if (game->enemies)
	{
		// Nettoie les chemins A* de chaque ennemi
		i = 0;
		while (i < game->enemies->count)
		{
			if (game->enemies->enemies[i].path)
			{
				free_path(game->enemies->enemies[i].path, game->enemies->enemies[i].path_length);
				game->enemies->enemies[i].path = NULL;
			}
			i++;
		}
		
		if (game->enemies->texture_guard)
			mlx_delete_texture(game->enemies->texture_guard);
		if (game->enemies->texture_officer)
			mlx_delete_texture(game->enemies->texture_officer);
		if (game->enemies->texture_dog)
			mlx_delete_texture(game->enemies->texture_dog);
		free(game->enemies);
		game->enemies = NULL;
	}
}
