/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

typedef struct s_sprite
{
	double	x;
	double	y;
	double	distance;
	int		enemy_index;
}	t_sprite;

static void	put_pixel_safe(mlx_image_t *img, int x, int y, uint32_t color)
{
	if (x >= 0 && y >= 0 && x < (int)img->width && y < (int)img->height)
	{
		int index = (y * img->width + x) * 4;
		img->pixels[index] = (color >> 24) & 0xFF;     // R
		img->pixels[index + 1] = (color >> 16) & 0xFF; // G
		img->pixels[index + 2] = (color >> 8) & 0xFF;  // B
		img->pixels[index + 3] = color & 0xFF;         // A
	}
}

static uint32_t	get_enemy_color(int enemy_type, int state)
{
	switch (enemy_type)
	{
		case 0: // Guard
			if (state == 3) // Dead
				return 0x800000FF; // Dark red
			else if (state == 2) // Attacking
				return 0xFF0000FF; // Bright red
			else if (state == 1) // Alert
				return 0xFF8000FF; // Orange
			else
				return 0x0000FFFF; // Blue
		case 1: // Officer
			if (state == 3) // Dead
				return 0x404040FF; // Dark gray
			else if (state == 2) // Attacking
				return 0xFF00FFFF; // Magenta
			else
				return 0x800080FF; // Purple
		case 2: // Dog
			if (state == 3) // Dead
				return 0x404020FF; // Dark brown
			else if (state == 2) // Attacking
				return 0xFFFF00FF; // Yellow
			else
				return 0x8B4513FF; // Brown
		default:
			return 0xFFFFFFFF; // White
	}
}

static void	draw_sprite_column(t_game *game, t_sprite *sprite, int stripe, 
	int draw_start_y, int draw_end_y, int sprite_height)
{
	t_enemy *enemy = &game->enemies->enemies[sprite->enemy_index];
	uint32_t color = get_enemy_color(enemy->enemy_type, enemy->state);
	
	(void)sprite_height; // Unused for now, but kept for future texture rendering
	
	// Simple solid color sprite for now
	for (int y = draw_start_y; y < draw_end_y; y++)
	{
		put_pixel_safe(game->img, stripe, y, color);
	}
	
	// Add a simple health bar above enemy if damaged
	if (enemy->hp < enemy->max_hp && enemy->is_alive)
	{
		int health_bar_y = draw_start_y - 10;
		int health_bar_width = 20;
		int health_bar_height = 3;
		int health_percentage = (enemy->hp * health_bar_width) / enemy->max_hp;
		
		// Background (black)
		for (int x = stripe - health_bar_width/2; x <= stripe + health_bar_width/2; x++)
		{
			for (int y = health_bar_y; y < health_bar_y + health_bar_height; y++)
			{
				put_pixel_safe(game->img, x, y, 0x000000FF);
			}
		}
		
		// Health (red to green gradient based on health)
		uint32_t health_color = enemy->hp > enemy->max_hp/2 ? 0x00FF00FF : 0xFF0000FF;
		for (int x = stripe - health_bar_width/2; x < stripe - health_bar_width/2 + health_percentage; x++)
		{
			for (int y = health_bar_y; y < health_bar_y + health_bar_height; y++)
			{
				put_pixel_safe(game->img, x, y, health_color);
			}
		}
	}
}

static void	render_sprite(t_game *game, t_sprite *sprite)
{
	// Transform sprite position relative to camera
	double sprite_x = sprite->x - game->player->pos.x;
	double sprite_y = sprite->y - game->player->pos.y;
	
	// Transform using the inverse camera matrix
	double inv_det = 1.0 / (game->player->plane.x * game->player->dir.y - 
		game->player->dir.x * game->player->plane.y);
	
	double transform_x = inv_det * (game->player->dir.y * sprite_x - 
		game->player->dir.x * sprite_y);
	double transform_y = inv_det * (-game->player->plane.y * sprite_x + 
		game->player->plane.x * sprite_y);
	
	// Don't draw if behind player
	if (transform_y <= 0)
		return;
	
	int sprite_screen_x = (int)((WINDOW_WIDTH / 2) * (1 + transform_x / transform_y));
	
	// Calculate sprite dimensions
	int sprite_height = abs((int)(WINDOW_HEIGHT / transform_y));
	int sprite_width = abs((int)(WINDOW_HEIGHT / transform_y));
	
	int draw_start_y = (WINDOW_HEIGHT - sprite_height) / 2;
	if (draw_start_y < 0) draw_start_y = 0;
	int draw_end_y = (WINDOW_HEIGHT + sprite_height) / 2;
	if (draw_end_y >= WINDOW_HEIGHT) draw_end_y = WINDOW_HEIGHT - 1;
	
	int draw_start_x = sprite_screen_x - sprite_width / 2;
	if (draw_start_x < 0) draw_start_x = 0;
	int draw_end_x = sprite_screen_x + sprite_width / 2;
	if (draw_end_x >= WINDOW_WIDTH) draw_end_x = WINDOW_WIDTH - 1;
	
	// Draw sprite columns
	for (int stripe = draw_start_x; stripe < draw_end_x; stripe++)
	{
		draw_sprite_column(game, sprite, stripe, draw_start_y, draw_end_y, sprite_height);
	}
}

static int	compare_sprites(const void *a, const void *b)
{
	t_sprite *sprite_a = (t_sprite *)a;
	t_sprite *sprite_b = (t_sprite *)b;
	
	// Sort by distance (farthest first for proper depth rendering)
	if (sprite_a->distance > sprite_b->distance)
		return (-1);
	else if (sprite_a->distance < sprite_b->distance)
		return (1);
	return (0);
}

void	render_enemies(t_game *game)
{
	t_sprite sprites[MAX_ENEMIES];
	int sprite_count = 0;
	
	// Collect all visible living enemies
	for (int i = 0; i < game->enemies->count; i++)
	{
		t_enemy *enemy = &game->enemies->enemies[i];
		
		if (!enemy->is_alive)
			continue;
		
		double dx = enemy->pos.x - game->player->pos.x;
		double dy = enemy->pos.y - game->player->pos.y;
		double distance = sqrt(dx * dx + dy * dy);
		
		// Don't render if too far
		if (distance > 20.0)
			continue;
		
		sprites[sprite_count].x = enemy->pos.x;
		sprites[sprite_count].y = enemy->pos.y;
		sprites[sprite_count].distance = distance;
		sprites[sprite_count].enemy_index = i;
		sprite_count++;
	}
	
	// Sort sprites by distance (farthest first)
	qsort(sprites, sprite_count, sizeof(t_sprite), compare_sprites);
	
	// Render all sprites
	for (int i = 0; i < sprite_count; i++)
	{
		render_sprite(game, &sprites[i]);
	}
}
