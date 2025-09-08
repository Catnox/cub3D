/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr      */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	game_update(void *param)
{
	t_game	*game;
	
	game = (t_game *)param;
	
	// Check for game over condition
	handle_game_over(game);
	
	// Si game over, on affiche seulement l'écran de fin
	if (game->game_over)
	{
		render_game_over_screen(game);
		return;
	}
	
	// Handle continuous input for smooth movement
	handle_continuous_input(game);
	
	// Update door animations
	update_doors(game);
	
	// Update weapon animations
	update_weapon(game);
	
	// Update enemy AI
	update_enemies(game);
	
	// Render everything
	render_frame(game);
	
	// Render enemies (sprites)
	render_enemies(game);
	
	// Render HUD on top
	render_hud(game);
	
	// Render weapon
	render_weapon(game);
}

void	game_loop(t_game *game)
{
	// Set up hooks
	mlx_key_hook(game->mlx, handle_keyboard, game);
	mlx_cursor_hook(game->mlx, handle_mouse, game);
	mlx_mouse_hook(game->mlx, handle_mouse_click, game);
	mlx_close_hook(game->mlx, (void *)mlx_close_window, game->mlx);
	mlx_loop_hook(game->mlx, game_update, game);
	
	// Main loop
	mlx_loop(game->mlx);
}
