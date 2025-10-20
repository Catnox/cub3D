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

#include "cub3d.h"

/* Main rendering function that draws a complete frame.
 * Clears the image buffer, draws floor and ceiling, then casts rays to render walls. */
void	render_frame(t_game *game)
{
	ft_memset(game->img->pixels, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
	draw_floor_ceiling(game);
	cast_rays(game);	
}

/* Draws the floor and ceiling as solid colored horizontal areas.
 * The upper half of the screen is filled with ceiling color,
 * the lower half with floor color based on map configuration. */
void	draw_floor_ceiling(t_game *game)
{
	int	x;
	int	y;
	int	floor_color;
	int	ceiling_color;
	
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

/* Calculates the drawing boundaries for a wall stripe based on ray distance.
 * Determines the height of the wall on screen and the start/end pixel positions.
 * Clamps the values to stay within the window boundaries. */
void	calculate_draw_bounds(t_ray *ray, int *line_height, int *draw_start, int *draw_end)
{
	*line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	
	*draw_start = -*line_height / 2 + WINDOW_HEIGHT / 2;
	if (*draw_start < 0)
		*draw_start = 0;
	*draw_end = *line_height / 2 + WINDOW_HEIGHT / 2;
	if (*draw_end >= WINDOW_HEIGHT)
		*draw_end = WINDOW_HEIGHT - 1;
}

/* Determines which texture to use based on the wall direction.
 * Returns the appropriate texture (north, south, east, west) depending on
 * which side of the wall was hit and the ray direction. */
mlx_texture_t	*get_wall_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (game->map->textures.east);
		else
			return (game->map->textures.west);
	}
	else
	{
		if (ray->step_y > 0)
			return (game->map->textures.south);
		else
			return (game->map->textures.north);
	}
}

/* Main wall drawing function that coordinates all wall rendering.
 * Calculates boundaries, selects textures, and delegates to appropriate drawing functions. */
void	draw_walls(t_game *game, int x, t_ray *ray)
{
	int				line_height;
	int				draw_start;
	int				draw_end;
	mlx_texture_t	*texture;
	
	calculate_draw_bounds(ray, &line_height, &draw_start, &draw_end);
	texture = get_wall_texture(game, ray);
	
	// If no texture loaded, fall back to colored walls
	if (!texture)
	{
		draw_colored_wall(game, x, ray, draw_start, draw_end);
		return;
	}
	
	// Draw the wall stripe with texture
	draw_textured_wall(game, x, ray, texture, line_height, draw_start, draw_end);
}
