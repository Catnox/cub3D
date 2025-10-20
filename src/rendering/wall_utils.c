/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 15:30:00 by radubos           #+#    #+#             */
/*   Updated: 2025/08/31 15:30:00 by radubos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* Calculates the X coordinate in the texture for the current wall stripe.
 * Determines the exact position on the wall where the ray hit and maps it
 * to the corresponding texture column. Handles texture flipping for correct orientation. */
int	calculate_texture_x(t_game *game, t_ray *ray, mlx_texture_t *texture)
{
	double	wall_x;
	int		tex_x;

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
	
	return (tex_x);
}

/* Initializes color components from texture pixels at given index.
 * Extracts RGBA values from texture data for both RGB and RGBA formats. */
void	init_color_components(mlx_texture_t *texture, int pixel_index, 
							t_color_components *color)
{
	color->r = texture->pixels[pixel_index];
	color->g = texture->pixels[pixel_index + 1];
	color->b = texture->pixels[pixel_index + 2];
	if (texture->bytes_per_pixel == 4)
		color->a = texture->pixels[pixel_index + 3];
	else
		color->a = 0xFF;
}

/* Extracts a pixel color from a texture at the specified coordinates.
 * Handles both RGB and RGBA texture formats. Returns a 32-bit RGBA color value.
 * Includes bounds checking and returns debug color for out-of-bounds access. */
uint32_t	get_texture_color(mlx_texture_t *texture, int tex_x, int tex_y)
{
	int					pixel_index;
	uint32_t			color;
	t_color_components	rgba;

	pixel_index = (tex_y * texture->width + tex_x) * texture->bytes_per_pixel;
	rgba.r = 0;
	rgba.g = 0;
	rgba.b = 0;
	rgba.a = 0xFF;
	
	if (pixel_index >= 0 && pixel_index < (int)(texture->width * texture->height * texture->bytes_per_pixel))
	{
		if (texture->bytes_per_pixel == 4 || texture->bytes_per_pixel == 3)
		{
			init_color_components(texture, pixel_index, &rgba);
			color = (rgba.r << 24) | (rgba.g << 16) | (rgba.b << 8) | rgba.a;
		}
		else
			color = 0xFFFFFFFF;
	}
	else
		color = 0xFF00FFFF;
	
	return (color);
}

/* Draws a textured wall stripe by sampling the texture vertically.
 * Maps each screen pixel to the corresponding texture pixel and draws
 * the wall with proper texture scaling and positioning. */
void	draw_textured_wall(t_game *game, int x, t_ray *ray, mlx_texture_t *texture, 
							int line_height, int draw_start, int draw_end)
{
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;
	int		y;

	tex_x = calculate_texture_x(game, ray, texture);
	step = 1.0 * texture->height / line_height;
	tex_pos = (draw_start - WINDOW_HEIGHT / 2 + line_height / 2) * step;
	
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = (int)tex_pos & (texture->height - 1);
		tex_pos += step;
		mlx_put_pixel(game->img, x, y, get_texture_color(texture, tex_x, tex_y));
		y++;
	}
}

/* Draws a solid colored wall stripe when no texture is available.
 * Uses different colors for each wall direction (red=east, blue=west, 
 * green=south, white=north) for debugging purposes. */
void	draw_colored_wall(t_game *game, int x, t_ray *ray, int draw_start, int draw_end)
{
	uint32_t	color;
	int			y;

	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			color = 0xFF0000FF;
		else
			color = 0x0000FFFF;
	}
	else
	{
		if (ray->step_y > 0)
			color = 0x00FF00FF;
		else
			color = 0xFFFFFFFF;
	}
	
	y = draw_start;
	while (y <= draw_end)
	{
		mlx_put_pixel(game->img, x, y, color);
		y++;
	}
}
