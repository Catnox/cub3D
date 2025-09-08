/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	init_crosshair_settings(t_hud *hud)
{
	hud->crosshair_size = 20;
	hud->crosshair_thickness = 2;
	hud->crosshair_color.r = 255;
	hud->crosshair_color.g = 255;
	hud->crosshair_color.b = 255;
	hud->crosshair_color.a = 255;
}

int	init_hud(t_game *game)
{
	game->hud = malloc(sizeof(t_hud));
	if (!game->hud)
		return (0);
	
	init_crosshair_settings(game->hud);
	
	// Create HUD images with transparency
	game->hud->crosshair = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	game->hud->health_bar = mlx_new_image(game->mlx, 200, 20);
	game->hud->ammo_counter = mlx_new_image(game->mlx, 100, 30);
	game->hud->score_display = mlx_new_image(game->mlx, 150, 25);
	
	if (!game->hud->crosshair || !game->hud->health_bar || 
		!game->hud->ammo_counter || !game->hud->score_display)
		return (0);
	
	// Set HUD images to overlay on top of game
	if (mlx_image_to_window(game->mlx, game->hud->crosshair, 0, 0) < 0)
		return (0);
	if (mlx_image_to_window(game->mlx, game->hud->health_bar, 10, 10) < 0)
		return (0);
	if (mlx_image_to_window(game->mlx, game->hud->ammo_counter, 
		WINDOW_WIDTH - 110, WINDOW_HEIGHT - 40) < 0)
		return (0);
	if (mlx_image_to_window(game->mlx, game->hud->score_display, 
		WINDOW_WIDTH - 160, 10) < 0)
		return (0);
	
	return (1);
}

void	update_hud(t_game *game)
{
	// Clear previous HUD elements
	ft_memset(game->hud->crosshair->pixels, 0, 
		game->hud->crosshair->width * game->hud->crosshair->height * 4);
	ft_memset(game->hud->health_bar->pixels, 0, 
		game->hud->health_bar->width * game->hud->health_bar->height * 4);
	ft_memset(game->hud->ammo_counter->pixels, 0, 
		game->hud->ammo_counter->width * game->hud->ammo_counter->height * 4);
	ft_memset(game->hud->score_display->pixels, 0, 
		game->hud->score_display->width * game->hud->score_display->height * 4);
}

void	render_hud(t_game *game)
{
	update_hud(game);
	draw_crosshair(game);
	draw_health_bar(game);
	draw_ammo_counter(game);
	draw_score_display(game);
}

void	cleanup_hud(t_game *game)
{
	if (game->hud)
	{
		if (game->hud->crosshair)
			mlx_delete_image(game->mlx, game->hud->crosshair);
		if (game->hud->health_bar)
			mlx_delete_image(game->mlx, game->hud->health_bar);
		if (game->hud->ammo_counter)
			mlx_delete_image(game->mlx, game->hud->ammo_counter);
		if (game->hud->score_display)
			mlx_delete_image(game->mlx, game->hud->score_display);
		free(game->hud);
		game->hud = NULL;
	}
}
