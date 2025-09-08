/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define MINIMAP_SIZE 200
#define MINIMAP_SCALE 8
#define MINIMAP_X 20
#define MINIMAP_Y 20

static uint32_t	get_color(char c)
{
	if (c == '1')
		return (0xFF000000);  // Black for walls
	else if (c == 'D')
		return (0x8B4513FF);  // Brown for doors
	else if (c == '0')
		return (0xFFFFFFFF);  // White for empty space
	return (0x808080FF);      // Gray for unknown
}

static void	draw_minimap_cell(t_game *game, int map_x, int map_y, int screen_x, int screen_y)
{
	uint32_t	color;
	int			i, j;
	
	if (map_x < 0 || map_x >= game->map->width || 
		map_y < 0 || map_y >= game->map->height)
		color = 0xFF000000;  // Black for out of bounds
	else
		color = get_color(game->map->grid[map_y][map_x]);
	
	// Draw a small square for each map cell
	for (i = 0; i < MINIMAP_SCALE && screen_x + i < MINIMAP_X + MINIMAP_SIZE; i++)
	{
		for (j = 0; j < MINIMAP_SCALE && screen_y + j < MINIMAP_Y + MINIMAP_SIZE; j++)
		{
			if (screen_x + i >= MINIMAP_X && screen_y + j >= MINIMAP_Y)
				mlx_put_pixel(game->img, screen_x + i, screen_y + j, color);
		}
	}
}

static void	draw_player_on_minimap(t_game *game)
{
	int	player_screen_x, player_screen_y;
	int	i, j;
	
	// Calculate player position on minimap
	player_screen_x = MINIMAP_X + (int)(game->player->pos.x * MINIMAP_SCALE);
	player_screen_y = MINIMAP_Y + (int)(game->player->pos.y * MINIMAP_SCALE);
	
	// Draw player as red dot
	for (i = -2; i <= 2; i++)
	{
		for (j = -2; j <= 2; j++)
		{
			if (player_screen_x + i >= MINIMAP_X && 
				player_screen_x + i < MINIMAP_X + MINIMAP_SIZE &&
				player_screen_y + j >= MINIMAP_Y && 
				player_screen_y + j < MINIMAP_Y + MINIMAP_SIZE)
			{
				mlx_put_pixel(game->img, player_screen_x + i, player_screen_y + j, 0xFF0000FF);
			}
		}
	}
	
	// Draw direction arrow
	int	arrow_x = player_screen_x + (int)(game->player->dir.x * 8);
	int	arrow_y = player_screen_y + (int)(game->player->dir.y * 8);
	
	if (arrow_x >= MINIMAP_X && arrow_x < MINIMAP_X + MINIMAP_SIZE &&
		arrow_y >= MINIMAP_Y && arrow_y < MINIMAP_Y + MINIMAP_SIZE)
	{
		mlx_put_pixel(game->img, arrow_x, arrow_y, 0x00FF00FF);  // Green arrow
	}
}

void	draw_minimap(t_game *game)
{
	int	map_x, map_y;
	int	screen_x, screen_y;
	
	// Draw minimap background (semi-transparent black border)
	for (int i = MINIMAP_X - 2; i < MINIMAP_X + MINIMAP_SIZE + 2; i++)
	{
		for (int j = MINIMAP_Y - 2; j < MINIMAP_Y + MINIMAP_SIZE + 2; j++)
		{
			if (i >= 0 && i < WINDOW_WIDTH && j >= 0 && j < WINDOW_HEIGHT)
				mlx_put_pixel(game->img, i, j, 0x000000AA);
		}
	}
	
	// Draw map cells
	for (map_y = 0; map_y < game->map->height; map_y++)
	{
		for (map_x = 0; map_x < game->map->width; map_x++)
		{
			screen_x = MINIMAP_X + map_x * MINIMAP_SCALE;
			screen_y = MINIMAP_Y + map_y * MINIMAP_SCALE;
			
			if (screen_x < MINIMAP_X + MINIMAP_SIZE && 
				screen_y < MINIMAP_Y + MINIMAP_SIZE)
			{
				draw_minimap_cell(game, map_x, map_y, screen_x, screen_y);
			}
		}
	}
	
	// Draw player
	draw_player_on_minimap(game);
	
	// Draw enemies on minimap
	for (int i = 0; i < game->enemies->count; i++)
	{
		t_enemy *enemy = &game->enemies->enemies[i];
		if (!enemy->is_alive)
			continue;
		
		int enemy_screen_x = MINIMAP_X + (int)(enemy->pos.x * MINIMAP_SCALE);
		int enemy_screen_y = MINIMAP_Y + (int)(enemy->pos.y * MINIMAP_SCALE);
		
		// Draw enemy as red dot
		uint32_t enemy_color = 0xFF0000FF; // Red
		if (enemy->state == 3) // Dead
			enemy_color = 0x800000FF; // Dark red
		else if (enemy->state == 2) // Attacking
			enemy_color = 0xFF4000FF; // Bright red-orange
		else if (enemy->state == 1) // Alert
			enemy_color = 0xFF8000FF; // Orange
		
		// Draw 2x2 pixel enemy
		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				int px = enemy_screen_x + dx;
				int py = enemy_screen_y + dy;
				if (px >= MINIMAP_X && px < MINIMAP_X + MINIMAP_SIZE &&
					py >= MINIMAP_Y && py < MINIMAP_Y + MINIMAP_SIZE)
				{
					mlx_put_pixel(game->img, px, py, enemy_color);
				}
			}
		}
	}
}
