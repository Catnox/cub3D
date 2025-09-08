/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	create_weapon_frame(t_game *game, int frame_index)
{
	t_color	weapon_color = {139, 69, 19, 255};  // Brown weapon
	t_color	fire_color = {255, 255, 0, 255};    // Yellow fire
	int		weapon_width = 200;
	int		weapon_height = 150;
	int		i, j;

	game->player->weapon.frames[frame_index] = mlx_new_image(game->mlx, 
		weapon_width, weapon_height);
	if (!game->player->weapon.frames[frame_index])
		return;

	// Clear frame with transparent pixels
	ft_memset(game->player->weapon.frames[frame_index]->pixels, 0, 
		weapon_width * weapon_height * 4);

	// Draw basic weapon shape
	for (i = 0; i < weapon_width; i++)
	{
		for (j = 0; j < weapon_height; j++)
		{
			// Draw weapon barrel (rectangle in center)
			if (i >= weapon_width / 3 && i <= 2 * weapon_width / 3 && 
				j >= weapon_height / 2)
			{
				int pixel_index = (j * weapon_width + i) * 4;
				game->player->weapon.frames[frame_index]->pixels[pixel_index] = weapon_color.r;
				game->player->weapon.frames[frame_index]->pixels[pixel_index + 1] = weapon_color.g;
				game->player->weapon.frames[frame_index]->pixels[pixel_index + 2] = weapon_color.b;
				game->player->weapon.frames[frame_index]->pixels[pixel_index + 3] = weapon_color.a;
			}
			// Draw muzzle flash for shooting frames
			else if (frame_index > 0 && frame_index < 4 && 
				i >= weapon_width / 3 && i <= 2 * weapon_width / 3 && 
				j < weapon_height / 2 && j > weapon_height / 4)
			{
				int pixel_index = (j * weapon_width + i) * 4;
				game->player->weapon.frames[frame_index]->pixels[pixel_index] = fire_color.r;
				game->player->weapon.frames[frame_index]->pixels[pixel_index + 1] = fire_color.g;
				game->player->weapon.frames[frame_index]->pixels[pixel_index + 2] = fire_color.b;
				game->player->weapon.frames[frame_index]->pixels[pixel_index + 3] = fire_color.a;
			}
		}
	}
}

int	init_weapon(t_game *game)
{
	int	i;

	// Initialize weapon properties
	game->player->weapon.current_frame = 0;
	game->player->weapon.frame_count = 5;  // 1 idle + 4 shooting frames
	game->player->weapon.is_shooting = 0;
	game->player->weapon.animation_timer = 0.0;
	game->player->weapon.frame_duration = 0.1;  // 100ms per frame
	game->player->weapon.damage = 25;
	game->player->weapon.ammo_per_shot = 1;

	// Create weapon animation frames
	for (i = 0; i < game->player->weapon.frame_count; i++)
	{
		create_weapon_frame(game, i);
		if (!game->player->weapon.frames[i])
			return (0);
	}

	// Display initial weapon frame
	if (mlx_image_to_window(game->mlx, game->player->weapon.frames[0], 
		(WINDOW_WIDTH - 200) / 2, WINDOW_HEIGHT - 150) < 0)
		return (0);

	return (1);
}

void	update_weapon(t_game *game)
{
	struct timeval	current_time;
	static double	last_update = 0.0;
	double			current_seconds;
	double			delta_time;

	gettimeofday(&current_time, NULL);
	current_seconds = current_time.tv_sec + current_time.tv_usec / 1000000.0;
	
	if (last_update == 0.0)
		last_update = current_seconds;
	
	delta_time = current_seconds - last_update;
	last_update = current_seconds;

	if (game->player->weapon.is_shooting)
	{
		game->player->weapon.animation_timer += delta_time;
		
		if (game->player->weapon.animation_timer >= game->player->weapon.frame_duration)
		{
			game->player->weapon.animation_timer = 0.0;
			game->player->weapon.current_frame++;
			
			// End of shooting animation
			if (game->player->weapon.current_frame >= game->player->weapon.frame_count)
			{
				game->player->weapon.current_frame = 0;
				game->player->weapon.is_shooting = 0;
			}
		}
	}
}

void	render_weapon(t_game *game)
{
	int	i;

	// Hide all weapon frames first
	for (i = 0; i < game->player->weapon.frame_count; i++)
	{
		game->player->weapon.frames[i]->enabled = false;
	}

	// Show current frame
	game->player->weapon.frames[game->player->weapon.current_frame]->enabled = true;
}

void	shoot_weapon(t_game *game)
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

	// Here you would add raycasting to detect what was hit
	// For now, just a placeholder for hitting logic
	printf("BANG! Shot fired. Ammo remaining: %d\n", game->player->ammo);
}

void	reload_weapon(t_game *game)
{
	if (game->player->ammo < game->player->max_ammo)
	{
		game->player->ammo = game->player->max_ammo;
		printf("Weapon reloaded! Ammo: %d\n", game->player->ammo);
	}
}

void	cleanup_weapon(t_game *game)
{
	int	i;

	for (i = 0; i < game->player->weapon.frame_count; i++)
	{
		if (game->player->weapon.frames[i])
		{
			mlx_delete_image(game->mlx, game->player->weapon.frames[i]);
			game->player->weapon.frames[i] = NULL;
		}
	}
}
