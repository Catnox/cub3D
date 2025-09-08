/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	put_pixel(mlx_image_t *img, int x, int y, t_color color)
{
	int	pixel_index;

	if (x >= 0 && y >= 0 && x < (int)img->width && y < (int)img->height)
	{
		pixel_index = (y * img->width + x) * 4;
		img->pixels[pixel_index] = color.r;
		img->pixels[pixel_index + 1] = color.g;
		img->pixels[pixel_index + 2] = color.b;
		img->pixels[pixel_index + 3] = color.a;
	}
}

static void	draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, t_color color)
{
	int	dx = abs(x1 - x0);
	int	dy = abs(y1 - y0);
	int	sx = (x0 < x1) ? 1 : -1;
	int	sy = (y0 < y1) ? 1 : -1;
	int	err = dx - dy;
	int	e2;

	while (1)
	{
		put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void	draw_crosshair(t_game *game)
{
	int	center_x = WINDOW_WIDTH / 2;
	int	center_y = WINDOW_HEIGHT / 2;
	int	half_size = game->hud->crosshair_size / 2;
	int	thickness = game->hud->crosshair_thickness;
	t_color	color = game->hud->crosshair_color;

	// Draw horizontal line
	for (int i = -thickness / 2; i <= thickness / 2; i++)
	{
		draw_line(game->hud->crosshair, 
			center_x - half_size, center_y + i,
			center_x + half_size, center_y + i, color);
	}

	// Draw vertical line  
	for (int i = -thickness / 2; i <= thickness / 2; i++)
	{
		draw_line(game->hud->crosshair,
			center_x + i, center_y - half_size,
			center_x + i, center_y + half_size, color);
	}
}

void	draw_health_bar(t_game *game)
{
	t_color	bg_color = {50, 50, 50, 200};
	t_color	health_color = {255, 0, 0, 255};
	t_color	border_color = {255, 255, 255, 255};
	int		health_width;
	int		i, j;

	// Draw border
	for (i = 0; i < (int)game->hud->health_bar->width; i++)
	{
		put_pixel(game->hud->health_bar, i, 0, border_color);
		put_pixel(game->hud->health_bar, i, game->hud->health_bar->height - 1, border_color);
	}
	for (j = 0; j < (int)game->hud->health_bar->height; j++)
	{
		put_pixel(game->hud->health_bar, 0, j, border_color);
		put_pixel(game->hud->health_bar, game->hud->health_bar->width - 1, j, border_color);
	}

	// Draw background
	for (i = 1; i < (int)game->hud->health_bar->width - 1; i++)
	{
		for (j = 1; j < (int)game->hud->health_bar->height - 1; j++)
		{
			put_pixel(game->hud->health_bar, i, j, bg_color);
		}
	}

	// Draw health
	health_width = ((game->hud->health_bar->width - 2) * game->player->hp) / game->player->max_hp;
	for (i = 1; i < health_width + 1; i++)
	{
		for (j = 1; j < (int)game->hud->health_bar->height - 1; j++)
		{
			put_pixel(game->hud->health_bar, i, j, health_color);
		}
	}
}

void	draw_ammo_counter(t_game *game)
{
	t_color	bg_color = {0, 0, 0, 150};
	t_color	text_color = {255, 255, 0, 255};
	char	ammo_text[20];
	int		i, j;

	// Draw background
	for (i = 0; i < (int)game->hud->ammo_counter->width; i++)
	{
		for (j = 0; j < (int)game->hud->ammo_counter->height; j++)
		{
			put_pixel(game->hud->ammo_counter, i, j, bg_color);
		}
	}

	// Simple text rendering - just draw some pixels to represent numbers
	// In a real implementation, you'd load a font texture
	snprintf(ammo_text, sizeof(ammo_text), "AMMO: %d", game->player->ammo);
	
	// Draw simple pixel "text" representation
	for (i = 5; i < 95; i += 2)
	{
		for (j = 5; j < 25; j += 3)
		{
			if ((i + j) % 8 < 4)  // Simple pattern
				put_pixel(game->hud->ammo_counter, i, j, text_color);
		}
	}
}

void	draw_score_display(t_game *game)
{
	t_color	bg_color = {0, 0, 0, 150};
	t_color	text_color = {0, 255, 0, 255};
	char	score_text[20];
	int		i, j;

	// Draw background
	for (i = 0; i < (int)game->hud->score_display->width; i++)
	{
		for (j = 0; j < (int)game->hud->score_display->height; j++)
		{
			put_pixel(game->hud->score_display, i, j, bg_color);
		}
	}

	// Simple text rendering - just draw some pixels to represent numbers
	snprintf(score_text, sizeof(score_text), "SCORE: %d", game->player->score);
	
	// Draw simple pixel "text" representation
	for (i = 5; i < 145; i += 2)
	{
		for (j = 5; j < 20; j += 2)
		{
			if ((i * j) % 12 < 6)  // Simple pattern
				put_pixel(game->hud->score_display, i, j, text_color);
		}
	}
}
