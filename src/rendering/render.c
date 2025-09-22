/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	render_frame(t_game *game)
{
	// Clear image
	ft_memset(game->img->pixels, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
	
	// Draw floor and ceiling
	draw_floor_ceiling(game);
	
	// Cast rays and draw walls
	cast_rays(game);
	
	/* BONUS FEATURE DISABLED - minimap */
	// draw_minimap(game);
}

void	draw_floor_ceiling(t_game *game)
{
	int	x;
	int	y;
	int	floor_color;
	int	ceiling_color;
	
	// Convert colors to RGBA format for MLX42
	floor_color = (game->map->floor_color.r << 24) | (game->map->floor_color.g << 16) | 
				  (game->map->floor_color.b << 8) | game->map->floor_color.a;
	ceiling_color = (game->map->ceiling_color.r << 24) | (game->map->ceiling_color.g << 16) | 
					(game->map->ceiling_color.b << 8) | game->map->ceiling_color.a;
	
	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				mlx_put_pixel(game->img, x, y, ceiling_color);
			else
				mlx_put_pixel(game->img, x, y, floor_color);
			x++;
		}
		y++;
	}
}

void	draw_walls(t_game *game, int x, t_ray *ray)
{
	int	line_height;
	int	draw_start;
	int	draw_end;
	int	y;
	mlx_texture_t	*texture;
	int	tex_x;
	int	tex_y;
	double	step;
	double	tex_pos;
	uint32_t	color;
	
	// Calculate height of line to draw on screen
	line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	
	// Calculate lowest and highest pixel to fill in current stripe
	draw_start = -line_height / 2 + WINDOW_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + WINDOW_HEIGHT / 2;
	if (draw_end >= WINDOW_HEIGHT)
		draw_end = WINDOW_HEIGHT - 1;
	
	// Get appropriate texture based on wall direction
	if (game->map->grid[ray->map_y][ray->map_x] == 'D')
	{
		// It's a door, use door texture
		texture = game->map->textures.door;
	}
	else if (ray->side == 0) // North-South wall
	{
		if (ray->step_x > 0)
			texture = game->map->textures.east; // East wall
		else
			texture = game->map->textures.west; // West wall
	}
	else // East-West wall
	{
		if (ray->step_y > 0)
			texture = game->map->textures.south; // South wall
		else
			texture = game->map->textures.north; // North wall
	}
	
	// If no texture loaded, fall back to colored walls
	if (!texture)
	{
		if (ray->side == 0) // North-South wall
		{
			if (ray->step_x > 0)
				color = 0xFF0000FF; // Red for East
			else
				color = 0x0000FFFF; // Blue for West
		}
		else // East-West wall
		{
			if (ray->step_y > 0)
				color = 0x00FF00FF; // Green for South
			else
				color = 0xFFFFFFFF; // White for North
		}
		
		y = draw_start;
		while (y <= draw_end)
		{
			mlx_put_pixel(game->img, x, y, color);
			y++;
		}
		return;
	}
	
	// Calculate texture x coordinate
	double wall_x;
	if (ray->side == 0)
		wall_x = game->player->pos.y + ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = game->player->pos.x + ray->perp_wall_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	
	tex_x = (int)(wall_x * (double)texture->width);
	if (ray->side == 0 && ray->dir.x > 0)
		tex_x = texture->width - tex_x - 1;
	if (ray->side == 1 && ray->dir.y < 0)
		tex_x = texture->width - tex_x - 1;
	
	// How much to increase the texture coordinate per screen pixel
	step = 1.0 * texture->height / line_height;
	
	// Starting texture coordinate
	tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
	
	// Draw the wall stripe with texture
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = (int)tex_pos & (texture->height - 1);
		tex_pos += step;
		
		// Get pixel color from texture
		int pixel_index = (tex_y * texture->width + tex_x) * texture->bytes_per_pixel;
		if (pixel_index >= 0 && pixel_index < (int)(texture->width * texture->height * texture->bytes_per_pixel))
		{
			if (texture->bytes_per_pixel == 4) // RGBA
			{
				uint8_t r = texture->pixels[pixel_index];
				uint8_t g = texture->pixels[pixel_index + 1];
				uint8_t b = texture->pixels[pixel_index + 2];
				uint8_t a = texture->pixels[pixel_index + 3];
				color = (r << 24) | (g << 16) | (b << 8) | a;
			}
			else if (texture->bytes_per_pixel == 3) // RGB
			{
				uint8_t r = texture->pixels[pixel_index];
				uint8_t g = texture->pixels[pixel_index + 1];
				uint8_t b = texture->pixels[pixel_index + 2];
				color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
			}
			else
			{
				color = 0xFFFFFFFF; // Default white
			}
		}
		else
		{
			color = 0xFF00FFFF; // Magenta for debug (out of bounds)
		}
		
		mlx_put_pixel(game->img, x, y, color);
		y++;
	}
}
